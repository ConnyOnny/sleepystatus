#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef CHARGE_STR
#define CHARGE_STR "⚡"
#endif

#ifndef NO_CHARGE_STR
#define NO_CHARGE_STR "_"
#endif

#ifndef ACPI_STATUS_FILE
#error "Need the path to the battery status file for initial state check"
#endif

#ifndef ACPI_CHARGE_STR
#error "Need to define the acpi string for adapter plugging"
#endif

#ifndef ACPI_NO_CHARGE_STR
#error "Need to define the acpi string for adapter unplugging"
#endif

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

void *battery_status(void *arg) {
	size_t tid = (size_t)arg;
	// initial lookup and setting
	FILE* statusfile = fopen(ACPI_STATUS_FILE,"r");
	if (!statusfile) {
		perror("battery_status - open status file");
	} else {
		int initial_status;
		if (fscanf(statusfile,"%d",&initial_status) != 1) {
			fprintf(stderr,"battery_status - read status file\n");
		} else {
			notify_update(tid,initial_status?CHARGE_STR:NO_CHARGE_STR);
		}
	}
	// listen to acpi events for power (un)plug events
	int filedes[2];
	char buf[BUFSIZE];
	if (pipe(filedes) == -1) {
		perror("battery_status - pipe");
		goto FAIL;
	}
	pid_t pid = fork();
	// explanation: http://www.microhowto.info/howto/capture_the_output_of_a_child_process_in_c.html
	if (pid == -1) {
		perror("battery_status - fork");
		goto FAIL;
	} else if (pid == 0) {
		// child: acpi_listen
		while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
		close(filedes[1]);
		close(filedes[0]);
		execl("/usr/bin/acpi_listen","acpi_listen");
		// should never be reached
		perror("battery_status - exec");
		goto FAIL;
	}
	// parent: analyse output of child
	while (1) {
		ssize_t count = read(filedes[0],buf,sizeof(buf));
		if (count == -1) {
			if (errno == EINTR) {
				continue;
			} else {
				perror("battery_status - read error");
				goto FAIL;
			}
		} else if (count == 0) {
			// no more input
			// this should not happen
			fprintf(stderr,"battery_status - acpi_listen has stopped its output.\n");
			break;
		} else {
			// input
			if (strstr(buf, ACPI_CHARGE_STR)) {
				notify_update(tid,CHARGE_STR);
			} else if (strstr(buf, ACPI_NO_CHARGE_STR)) {
				notify_update(tid,NO_CHARGE_STR);
			}
		}
	}
FAIL:
	notify_update(tid,"");
	return NULL;
}
