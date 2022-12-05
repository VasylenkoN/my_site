#include <stdlib.h>

#include "tarray.h"

static void my_error(const char * str) {
	fprintf(stderr, "ERROR! %s\n", str);
	exit(EXIT_FAILURE);
}

void tarray_init(TARRAY * arr, int num) {
	if (!(arr->res = (double *)calloc(num, sizeof(double)))) {
		my_error("Cannot allocate array!");
	}
	arr->size = num;
	if(pthread_rwlock_init(&(arr->lock), NULL)) {
		my_error("Cannot initialize rwlock!");
	}
}

void tarray_dest(TARRAY * arr) {
	free(arr->res);
	arr->res = NULL;
	pthread_rwlock_destroy(&(arr->lock));
}

void tarray_set(TARRAY * arr, int ind, double val) {
	pthread_rwlock_wrlock(&(arr->lock));
	arr->res[ind] = val;
	pthread_rwlock_unlock(&(arr->lock));
}

double tarray_get(TARRAY * arr, int ind) {
	double res;
	pthread_rwlock_rdlock(&(arr->lock));
	res = arr->res[ind];
	pthread_rwlock_unlock(&(arr->lock));
	return res;
}

void tarray_print(TARRAY * arr, FILE ** fptr) {
	int i;
	pthread_rwlock_rdlock(&(arr->lock));
	for (i = 0; i < arr->size; i++) {
			printf("%9.4f", arr->res[i]);
	}
	pthread_rwlock_unlock(&(arr->lock));
}