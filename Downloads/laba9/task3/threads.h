#ifndef MY_THREADS_H_
#define MY_THREADS_H_

#include "tarray.h"

typedef struct {
	int num_thread;
	int num_reps;
	TARRAY * arr;
} TARG;

void * write_thread(void * arg);
void * read_thread(void * arg);
void * detach_thread(void * arg);

#endif