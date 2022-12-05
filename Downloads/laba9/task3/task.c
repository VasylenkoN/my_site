#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "task.h"
#include "threads.h"

typedef struct {
	pthread_t * thread;
	TARG * arg;
} MAKE_RES;

static pthread_t make_detached(TARRAY * arr);
static MAKE_RES * make_writers(TARG * targ);
static MAKE_RES * make_readers(TARG * targ);

void task_solution(TPARAM * init) {
	pthread_t thread;
	TARG arg_writer, arg_reader;
	MAKE_RES * writer;
	MAKE_RES * reader;
	int i;
	
	printf("Work with %d writer threads\n", init->num_writers);
	printf("Work with %d reader threads\n", init->num_readers);
	printf("All threads make %d reaps\n", init->num_reps);
	printf("Initial array: ");
	tarray_print(init->arr, &stdout);
	printf("\n");
	thread = make_detached(init->arr);	
	//sleep(5);
	arg_writer.arr = init->arr;
	arg_writer.num_reps = init->num_reps;
	arg_writer.num_thread = init->num_writers;
	writer = make_writers(&arg_writer);
	arg_reader.arr = init->arr;
	arg_reader.num_reps = init->num_reps;
	arg_reader.num_thread = init->num_readers;
	reader = make_readers(&arg_reader);
	
	for (i = 0; i < init->num_writers; i++) {
		if (pthread_join(writer->thread[i], NULL) != 0) {
			fprintf(stderr, "Writer Thread %d Waiting Error\n", i);
			exit(EXIT_FAILURE);
		}
	}
	for (i = 0; i < init->num_readers; i++) {
		if (pthread_join(reader->thread[i], NULL) != 0) {
			fprintf(stderr, "Reader Thread %d Waiting Error\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	free(writer->thread);  free(writer->arg);  free(writer);
	free(reader->thread);  free(reader->arg);  free(reader);
	
	if(pthread_cancel(thread)) {
		fprintf(stderr, "ERROR! Cannot stop detached thread!\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Result array : ");
	tarray_print(init->arr, &stdout);
	printf("\n");
	sleep(5);
}


static pthread_t make_detached(TARRAY * arr) {
	pthread_attr_t attr;
	pthread_t thread;
	int a;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (!pthread_attr_getdetachstate(&attr, &a)) {
		printf("%d\t%d\t", a, PTHREAD_CREATE_DETACHED);
		if (a == PTHREAD_CREATE_DETACHED) {
			printf("Really detached\n");
		}
	}
	if (pthread_create(&thread, &attr, &detach_thread, (void*)arr)) {
		fprintf(stderr, "Error while Creation Detached Thread\n");
		exit(EXIT_FAILURE);
	}
	pthread_attr_destroy(&attr);
	
	return thread;
}


static MAKE_RES * make_writers(TARG * targ) {
	MAKE_RES * res;
	TARG * arg;
	int i;
	
	res = (MAKE_RES *) malloc(sizeof(MAKE_RES));
	if (!res) {
		fprintf(stderr, "Allocation memory error\n");
		exit(EXIT_FAILURE);
	}
	res->thread = (pthread_t *) calloc(targ->num_thread, sizeof(pthread_t));
	res->arg = (TARG *) calloc(targ->num_thread, sizeof(TARG));
	if ((res->thread == NULL) || (res->arg == NULL)) {
		fprintf(stderr, "Allocation memory error\n");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < targ->num_thread; i++) {
		res->arg[i].num_reps = targ->num_reps;
		res->arg[i].num_thread = i;
		res->arg[i].arr = targ->arr;
		if (pthread_create(&(res->thread[i]), NULL, &write_thread, &(res->arg[i]))) {
			fprintf(stderr, "Writer Thread %d Creation Error\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	return res;
}

static MAKE_RES * make_readers(TARG * targ) {
	MAKE_RES * res;
	TARG * arg;
	int i;
	
	res = (MAKE_RES *) malloc(sizeof(MAKE_RES));
	if (!res) {
		fprintf(stderr, "Allocation memory error\n");
		exit(EXIT_FAILURE);
	}
	res->thread = (pthread_t *) calloc(targ->num_thread, sizeof(pthread_t));
	res->arg = (TARG *) calloc(targ->num_thread, sizeof(TARG));
	if ((res->thread == NULL) || (res->arg == NULL)) {
		fprintf(stderr, "Allocation memory error\n");
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < targ->num_thread; i++) {
		res->arg[i].num_reps = targ->num_reps;
		res->arg[i].num_thread = i;
		res->arg[i].arr = targ->arr;
		if (pthread_create(&(res->thread[i]), NULL, &read_thread, &(res->arg[i]))) {
			fprintf(stderr, "Reader Thread %d Creation Error\n", i);
			exit(EXIT_FAILURE);
		}
	}
	
	return res;
}

