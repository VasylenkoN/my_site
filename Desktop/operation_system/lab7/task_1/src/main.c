#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//заданіе №1
//После
//вывода строки каждый поток «засыпает» на заданное количество секунд (например, от 0 до 2).
long WAIT_FOR_SECONDS = 2;
//новий поток
void * any_func(void * args) {
    for (int i = 0; i < 10; i++) {
        fprintf(stderr, "Child Thread. Iteration: %d\n", (i+1));
        sleep(WAIT_FOR_SECONDS);
    }
//    sleep(WAIT_FOR_SECONDS);
    return NULL;
}
int main(void) {
    pthread_t thread;
    int result;
    result = pthread_create(&thread, NULL, &any_func, NULL);
    if (result != 0) {
        fprintf(stderr, "Error\n");
        return 1;
    }
    for (int i = 0; i < 10; i++) {
        fprintf(stderr, "Parent Thread. Iteration: %d\n", (i+1));
        sleep(WAIT_FOR_SECONDS);
    }
//    fprintf(stderr, "Goodbye World\n");
//    while(1);
    return 0;
}