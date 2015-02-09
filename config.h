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

#define FUNCNAME mytext
#define TEXT "bäm⚡"
#include "functions/static_text.c"
#undef FUNCNAME
#undef TEXT

#define ACPI_CHARGE_STR    "ac_adapter ACPI0003:00 00000080 00000001"
#define ACPI_NO_CHARGE_STR "ac_adapter ACPI0003:00 00000080 00000000"
#define ACPI_STATUS_FILE "/sys/class/power_supply/ADP0/online"
#include "functions/battery_status.c"
#undef ACPI_CHARGE_STR
#undef ACPI_NO_CHARGE_STR
#undef ACPI_STATUS_FILE

static ptfunc_t writer_thread_functions [] = {
	battery_status,
	minute_clock,
	mytext,
	second_clock,
};

#endif /*include guard*/
