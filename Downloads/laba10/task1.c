#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 10
#define ITERATION 100

void *producer();
void *consumer();
int produce(int minimum_number, int max_number);
void consume(int item);
void append_to_buffer(int num);
int receive_from_buffer();
void check_if_error(int result, char *err);

int array[SIZE];
//int item;

sem_t empty_items;
sem_t full_items;
sem_t lock;
//😏
int main(void) {
    // другий нуль між потоками, якщо 1, то між процесами
    // третій аргумент - значення
    check_if_error(sem_init(&empty_items, 0, SIZE), "creating empty_items semaphore");
    check_if_error(sem_init(&lock, 0, 1), "creating lock semaphore");
    check_if_error(sem_init(&full_items, 0, 0), "creating full_items semaphore");

    pthread_t consumer_thread;
    pthread_t producer_thread;

    // створюємо потік споживача
    check_if_error(pthread_create(&consumer_thread, NULL, consumer, NULL), "creating consumer_thread");
    // створюємо потік виробника
    check_if_error(pthread_create(&producer_thread, NULL, producer, NULL), "creating producer_thread");

    check_if_error(pthread_join(consumer_thread, NULL), "joining consumer_thread");
    check_if_error(pthread_join(producer_thread, NULL), "joining producer_thread");

    sem_destroy(&empty_items);
    sem_destroy(&lock);
    sem_destroy(&full_items);

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

void append_to_buffer(int num) {
    for (int i = 0; i < SIZE; i++) {
        if (array[i] == 0) {
            array[i] = num;
            break;
        }
    }
}

int receive_from_buffer() {
    for (int i = 0; i < SIZE; i++) {
        if (array[i] != 0) {
            int save = array[i];
            array[i] = 0;
            return save;
        }
    }
}

int produce(int minimum_number, int max_number) {
    return rand() % (max_number + 1 - minimum_number) + minimum_number;
}

void consume(int item) {
    fprintf(stderr, "\n[consumer] Споживаємо продукт %d... Дуже смачно...\n", item);
}

// виробник
// TODO перейменувати і переробити на конвертацію аргс в мін та макс
void *producer() {
    int min = 1;
    int max = 50;
    for (int i = 0;i < ITERATION; i++) {
        int item = produce(min, max); // створити ресурс
        printf("\n[producer] Створено ресурс: %d\n", item);
        printf("[producer] Очікуємо на порожні комірки...\n");
        sem_wait(&empty_items); // чи є місце у буфері?
        printf("[producer] Дочекалися на порожні комірки...\n");
        sem_wait(&lock); // вхід до критичної секції
        append_to_buffer(item); // додати ресурс item до буферу
        sem_post(&lock); // вихід із критичної секції
        sem_post(&full_items); // повідомити споживачів, що є новий ресурс
    }
}

// споживач
void *consumer() {
    for (int i = 0; i < ITERATION; ++i) {
        printf("\t\t[consumer] Очікуємо, щоб буфер не був порожнім...\n");
        sem_wait(&full_items); // чи не порожній буфер?
        printf("\t\t[consumer] Буфер не порожній...\n");
        sem_wait(&lock); // вхід до критичної секції
        printf("\t\t[consumer] Забираємо ресурс...\n");
        int item = receive_from_buffer(); // забрати ресурс item із буферу
        sem_post(&lock); // вихід із критичної секції
        sem_post(&empty_items); // повідомити виробників, що в буфері є місце
        consume(item); // «спожити» ресурс
    }
}
