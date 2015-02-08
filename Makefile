CFLAGS=-std=gnu99 $(shell pkg-config --cflags x11)
LDLIBS=-pthread $(shell pkg-config --libs x11)

sleepystatus : sleepystatus.c functions/* config.h defs.h
	$(CC) $(CFLAGS) sleepystatus.c -o sleepystatus $(LDLIBS)

clean :
	rm -f sleepystatus
