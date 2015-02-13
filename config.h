#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "defs.h"
/*
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
*/
#define FUNCNAME my_date_clock
#define BUFSIZE 32
#define WAKEUPON 1
static const char* day_names[] = {"So","Mo","Di","Mi","Do","Fr","Sa"};
static const char* my_dst_str(int isdst) {
	if (isdst == 0) { // no dst, "winter time"
		return "❄";
	} else if (isdst < 0) { // don't know
		return "?DST?";
	} else { // dst, "summer time"
		return "☀";
	}
}
#define PRINTFLINE(t) snprintf(buf,BUFSIZE,"%s %d.%02d %d:%02d%s",day_names[t.tm_wday],t.tm_mday,t.tm_mon+1,t.tm_hour,t.tm_min,my_dst_str(t.tm_isdst))
#include "functions/config_clock.c"
#undef FUNCNAME
#undef BUFSIZE
#undef WAKEUPON
#undef PINTFLINE

#define FUNCNAME sep
#define TEXT " "
#include "functions/static_text.c"
#undef FUNCNAME
#undef TEXT

#define ACPI_CHARGE_STR    "ac_adapter ACPI0003:00 00000080 00000001"
#define ACPI_NO_CHARGE_STR "ac_adapter ACPI0003:00 00000080 00000000"
#define ACPI_STATUS_FILE "/sys/class/power_supply/ADP0/online"
#define NO_CHARGE_STR ""
#include "functions/battery_status.c"
#undef ACPI_CHARGE_STR
#undef ACPI_NO_CHARGE_STR
#undef ACPI_STATUS_FILE
#undef NO_CHARGE_STR

#define ACPI_BATTERY_LEVEL_FILE "/sys/class/power_supply/BAT0/energy_now"
#define ACPI_BATTERY_MAXLEVEL_FILE "/sys/class/power_supply/BAT0/energy_full"
#include "functions/battery_level.c"
#undef ACPI_BATTERY_LEVEL_FILE
#undef ACPI_BATTERY_MAXLEVEL_FILE

static ptfunc_t writer_thread_functions [] = {
	battery_level,
	battery_status,
	sep,
	my_date_clock,
};

#endif /*include guard*/
