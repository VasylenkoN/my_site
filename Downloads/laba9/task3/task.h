#ifndef TASK_H_
#define TASK_H_

#include "tarray.h"

typedef struct {
	int num_readers;
	int num_writers;
	int num_reps;
	TARRAY * arr;
} TPARAM;

void task_solution(TPARAM * init);

#endif