#ifndef FUNCNAME
#error "Must define function name"
#endif

#ifndef PRINTFLINE
#error "Must define the way to display things"
#endif

#ifndef BUFSIZE
#error "Must define buffer size"
#endif

#ifndef WAKEUPON
#error "Must define when to wake up\n0=>every second"
#endif

#include "../defs.h"
#include <stdio.h>
#include <time.h>

void* FUNCNAME (void* arg) {
	char buf[BUFSIZE];
	size_t tid = (size_t)arg;
	struct timespec t_now, t_wakeup;
	struct tm t;
	while (1) {
		// make string
		clock_gettime(CLOCK_REALTIME, &t_now);
		localtime_r(&t_now.tv_sec,&t);
		PRINTFLINE(t);
		notify_update(tid,buf);
		//schedule next wakeup
		switch (WAKEUPON) {
		case 0:
			t.tm_sec++;
			break;
		case 1:
			t.tm_sec=0;
			t.tm_min++;
			break;
		case 2:
			t.tm_sec=0;
			t.tm_min=0;
			t.tm_hour++;
			break;
		}
		t_wakeup.tv_sec = timelocal(&t);
		t_wakeup.tv_nsec = 0;
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&t_wakeup,NULL);
	}
}

