#ifndef TARRAY_C_
#define TARRAY_C_

#include <stdio.h>
#include <pthread.h>

typedef struct {
	double * res;
	int size;
	pthread_rwlock_t lock;
} TARRAY;

void tarray_init(TARRAY * arr, int num);
void tarray_dest(TARRAY * arr);
void tarray_set(TARRAY * arr, int ind, double val);
double tarray_get(TARRAY * arr, int ind);
void tarray_print(TARRAY * arr, FILE ** fptr);

#endif