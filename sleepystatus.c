#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include "config.h"

#include <X11/Xlib.h>
static Display *dpy;
void close_dpy(void) { XCloseDisplay(dpy); }

#define LAZYSTR_INITIAL_SIZE 16
typedef struct {
	char* data;
	size_t maxlen;
} lazystr;
void lazystr_init(lazystr* s) {
	s->data = malloc(LAZYSTR_INITIAL_SIZE*sizeof(char));
	// TODO: Error handling
	s->data[0]='\0';
	s->maxlen = LAZYSTR_INITIAL_SIZE-1;
}
void lazystr_fitatleast(lazystr* s, size_t len) {
	if (len > s->maxlen) {
		size_t newsize = (s->maxlen+1)<<1;
		while (newsize-1 < len) {
			newsize <<=1;
		}
		s->data = realloc(s->data,newsize*sizeof(char));
		// TODO: Error handling
		s->maxlen = newsize-1;
	}
}
void lazystr_set(lazystr* s, char const* o) {
	size_t olen = strlen(o);
	lazystr_fitatleast(s,olen);
	assert(s->maxlen >= olen);
	strncpy(s->data, o, olen);
	// just to be sure
	s->data[olen] = '\0';
}

#define n_writer_thread_functions (sizeof(writer_thread_functions) / sizeof(ptfunc_t))
pthread_t threads[n_writer_thread_functions];
lazystr buffers[n_writer_thread_functions];
lazystr output_buffer;

pthread_mutex_t update_lock;
void destroy_update_lock(void) { pthread_mutex_destroy(&update_lock); }

void setstatus(char const *str) {
        XStoreName(dpy, DefaultRootWindow(dpy), str);
        XSync(dpy, False);
}

void notify_update(size_t id, char const* str) {
	pthread_mutex_lock(&update_lock);
	if (id >= n_writer_thread_functions) {
		// TODO: Error handling
		return;
	}
	lazystr_set(buffers+id, str);
	size_t len = n_writer_thread_functions-1; // spaces in between
	for (size_t i=0; i<n_writer_thread_functions; i++) {
		len += strlen(buffers[i].data);
	}
	lazystr_fitatleast(&output_buffer, len);
	int pos = sprintf(output_buffer.data, "%s", buffers[0].data);
	for (size_t i=1; i<n_writer_thread_functions; i++) {
		/* also adds the separating space */
		pos += sprintf(output_buffer.data+pos, " %s", buffers[i].data);
	}
	assert(pos <= output_buffer.maxlen);
	setstatus(output_buffer.data);
	pthread_mutex_unlock(&update_lock);
}

int main (int argc, char** argv) {
	int rv; // for return value checks
	if (argc != 1) {
		fprintf(stderr,"There are no command line switches available. Configuration is done at compile-time.\n");
		exit(1);
	}
	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "cannot open display.\n");
		exit(1);
	}
	atexit(close_dpy);
	lazystr_init(&output_buffer);
	rv = pthread_mutex_init(&update_lock, NULL);
	if (rv != 0) {
		// TODO: Error handling
		exit(3);
	}
	atexit(destroy_update_lock);
	for (size_t i=0; i<n_writer_thread_functions; i++) {
		lazystr_init(&buffers[i]);
	}
	for (size_t i=0; i<n_writer_thread_functions; i++) {
		rv = pthread_create(&threads[i], NULL, writer_thread_functions[i], (void*)i);
		if (rv != 0) {
			// TODO: Error handling
			exit(4);
		}
	}
	while(1) {
		/* pause doesn't return on exiting signals */
		pause();
	}
	exit(5);
}
