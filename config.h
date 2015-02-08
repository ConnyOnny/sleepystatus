#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "defs.h"

#define FUNCNAME second_clock
#define BUFSIZE 32
#define WAKEUPON 0
#define PRINTFLINE(t) snprintf(buf,BUFSIZE,"%d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec)
#include "functions/config_clock.c"
#undef FUNCNAME
#undef BUFSIZE
#undef WAKEUPON
#undef PRINTFLINE

#define FUNCNAME minute_clock
#define BUFSIZE 8
#define WAKEUPON 1
#define PRINTFLINE(t) snprintf(buf,BUFSIZE,"%d:%02d",t.tm_hour,t.tm_min)
#include "functions/config_clock.c"
#undef FUNCNAME
#undef BUFSIZE
#undef WAKEUPON
#undef PRINTFLINE

static ptfunc_t writer_thread_functions [] = {
	minute_clock,
	second_clock,
	second_clock,
};

#endif /*include guard*/
