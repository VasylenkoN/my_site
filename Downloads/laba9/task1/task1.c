#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>

void * thread_increment(void * arg);
void * thread_decrement(void * arg);

static long global = 0;

int main(int argc, char ** argv) {
    long num = 300000;
    int am = 2;

    setbuf(stdout, NULL);
    int opt;
    while ((opt = getopt(argc, argv, "a:n:")) != -1) {
        switch (opt) {
            case 'a':
                am = atoi(optarg);
                break;
            case 'n':
                num = atoi(optarg);
                break;
        }
    }

    printf("Work with %ld iterations in %d pair of threads\n", num, am);
    printf("Initial global value: %ld\n", global);

    int res;
    pthread_t * tid1, * tid2;
    tid1 = (pthread_t*)calloc(am, sizeof(pthread_t));
    tid2 = (pthread_t*)calloc(am, sizeof(pthread_t));

    for(int i = 0; i < am; i++) {
        if (res = pthread_create(tid1+i, NULL, thread_increment, &num)) {
            fprintf(stderr, "Increment thread creation ERROR (%d)\n", res);
            exit(1);
        }
        if (res = pthread_create(tid2+i, NULL, thread_decrement, &num)) {
            fprintf(stderr, "Decrement thread creation ERROR (%d)\n", res);
            exit(1);
        }
    }

    for(int i = 0; i < am; i++) {
        if (pthread_join(*(tid1 + i), NULL)) {
            fprintf(stderr, "[1] JOIN ERROR\n");
            exit(2);
        }
        if (pthread_join(*(tid2 + i), NULL)) {
            fprintf(stderr, "[2] JOIN ERROR\n");
            exit(2);
        }
    }

    printf("Final global value: %ld\n", global);

    return 0;
}

void * thread_increment(void * arg) {
    long num = *(long*)arg;
    long i, local;
    for (i = 0; i < num; i++) {
        local = global;
        local++;
        global = local;
    }
    return NULL;
}

void * thread_decrement(void * arg) {
    long num = *(long*)arg;
    long i, local;
    for (i = 0; i < num; i++) {
        local = global;
        local--;
        global = local;
    }
    return NULL;
}
