#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "threads.h"
#include "tarray.h"

void * write_thread(void * arg) {
	TARG * targ = (TARG *)arg;
	int k, ind;
	double val;
	printf("\tWriter Thread %d is started\n", targ->num_thread);
	sleep(targ->num_thread);
	srand(time(NULL));
	for (k = 0; k < targ->num_reps; k++) {
		ind = rand() % targ->arr->size;
		val = (double)rand() / RAND_MAX;
		tarray_set(targ->arr, ind, val);
		printf("\tWriter Thread No %d, res[%d] = %g\n", targ->num_thread, ind, val);
		sleep(ind);
	}
	printf("\tWriter Thread %d is stopped\n", targ->num_thread);
	return NULL;
}

void * read_thread(void * arg) {
	TARG * targ = (TARG *)arg;
	int k, ind;
	double res;
	printf("\t\tReader Thread %d is started\n", targ->num_thread);
	sleep(targ->num_thread);
	srand(time(NULL));
	for (k = 0; k < targ->num_reps; k++) {
		ind = rand() % targ->arr->size;
		res = tarray_get(targ->arr, ind);
		printf("\t\tReader Thread No %d, res[%d] = %g\n", targ->num_thread, ind, res);
		sleep(ind);
	}
	printf("\t\tReader Thread %d is stopped\n", targ->num_thread);
	return NULL;
}

void * detach_thread(void * arg) {
	TARRAY arr = *(TARRAY*)arg;
	
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		fprintf(stderr, "Cannot change cancel state\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		fprintf(stderr, "Cannot change cancel type\n");
		exit(EXIT_FAILURE);
	}
	printf("\tDetached Thread is Started\n");	
	while (1) {
		printf("Current state:");
		tarray_print(&arr, &stdout);
		printf("\n");
		sleep(1);
	}
	printf("\tDetached Thread is Stopped\n");
	return NULL;
}