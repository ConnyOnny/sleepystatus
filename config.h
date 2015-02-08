#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "defs.h"
#include "functions/minute_clock.h"

static ptfunc_t writer_thread_functions [] = {
	minute_clock,
	minute_clock,
	minute_clock,
};

#endif /*include guard*/
