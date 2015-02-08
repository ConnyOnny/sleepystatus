#include "minute_clock.h"
#include <stdio.h>
#include <time.h>

#define BUFSIZE 16

void* minute_clock (void* args) {
	char buf[BUFSIZE];
	size_t tid = (size_t)args;
	struct timespec t;
	struct tm t2;
	struct timespec t3;
	while (1) {
		// make string
		clock_gettime(CLOCK_REALTIME, &t);
		localtime_r(&(t.tv_sec),&t2);
		snprintf(buf,BUFSIZE,"%d:%02d",t2.tm_hour,t2.tm_min);
		notify_update(tid,buf);
		// schedule next wakeup
		t2.tm_sec=0;
		t2.tm_min++;
		t3.tv_sec = timelocal(&t2);
		t3.tv_nsec = 0;
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t3, NULL);
	}
	return NULL;
}
