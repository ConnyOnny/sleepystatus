#ifndef FUNCNAME
#error "Must define function name"
#endif

#ifndef TEXT
#error "Must define text do display"
#endif

void *FUNCNAME (void* arg) {
	size_t tid = (size_t)arg;
	notify_update(tid,TEXT);
	return NULL;	
}
