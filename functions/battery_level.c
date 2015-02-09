#ifndef ACPI_BATTERY_LEVEL_FILE
#error "Need to know where to read the current battery level"
#endif

#ifndef ACPI_BATTERY_MAXLEVEL_FILE
#error "Need to know where to read the maximum battery level"
#endif

#include <stdio.h>

int read_int_from_file(char const* fname) {
	FILE *f = fopen(fname,"r");
	if (!f) {
		fprintf(stderr,"Can't open file %s\n",fname);
		return 0;
	}
	int ret;
	if (fscanf(f,"%d",&ret)!=1) {
		fprintf(stderr,"File doesn't begin with int: %s\n",fname);
		fclose(f);
		return 0;
	}
	fclose(f);
	return ret;
}

void *battery_level (void *arg) {
	size_t tid = (size_t)arg;
	char buf[16];
	// the prefix is there to indicate the max height of the bar
	sprintf(buf,"▕");
	size_t appendpos = strlen(buf);
	char *levels[] = {"▁","▂","▃","▄","▅","▆","▇","█"};
	int lastidx=-1;
	while (1) {
		int bat_cur = read_int_from_file(ACPI_BATTERY_LEVEL_FILE);
		int bat_max = read_int_from_file(ACPI_BATTERY_MAXLEVEL_FILE);
		int idx = bat_cur * 8 / bat_max;
		if (idx != lastidx) {
			if (idx == 8) {
				// battery 100% full, don't show anything
				notify_update(tid,"");
			} else {
				sprintf(buf+appendpos,"%s",levels[idx]);
				notify_update(tid,buf);
			}
		}
		lastidx=idx;
		system("sleep 5");
	}
}
