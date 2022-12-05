#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PROGRAM_EXECUTION_TIME

void *producer_function();
void *consumer_function();
void check_if_error(int result, char *err);

int global;
int flag = 0;

pthread_mutex_t mtx;
pthread_cond_t producer;
pthread_cond_t consumer;

int main(void) {
    pthread_t consumer_thread;
    pthread_t producer_thread;
    setbuf(stdout, NULL);

    global = 0;

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&producer, NULL);
    pthread_cond_init(&consumer, NULL);

//    start = clock();
    check_if_error(pthread_create(&consumer_thread, NULL, consumer_function, NULL), "creating consumer_thread");
    check_if_error(pthread_create(&producer_thread, NULL, producer_function, NULL), "creating producer_thread");

    sleep(PROGRAM_EXECUTION_TIME);

    pthread_cancel(consumer_thread);
    pthread_cancel(producer_thread);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&producer);
    pthread_cond_destroy(&consumer);

    exit(EXIT_SUCCESS);
}

void check_if_error(int result, char *err) {
    if (result != 0) {
        perror("Something is wrong. Maybe your life?\t");
        perror("Error occurred during");
        perror(err);
        exit(EXIT_FAILURE);
    }
}

void *producer_function() {
    while(1) {
//        usleep(6*1000*1000);
        sleep(1);
        while(flag == 1) {
            pthread_cond_wait(&producer, &mtx);
        }
        pthread_mutex_lock(&mtx);
        int minimum_number = 1;
        int max_number = 50;
        flag = 1;
        global = rand() % (max_number + 1 - minimum_number) + minimum_number;
        printf("\n[producer] Створено ресурс: %d\n", global);
        pthread_cond_signal(&consumer);
        pthread_mutex_unlock(&mtx);
    }
}

void *consumer_function() {
    while(1) {
        pthread_mutex_lock(&mtx);
        while(flag == 0) {
            pthread_cond_wait(&consumer, &mtx);
        }
        int value = global;
        global = 0;

        printf("\n\t\t[consumer] Забрано ресурс: %d\n", value);
        flag = 0;
        pthread_cond_signal(&producer);
        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}
