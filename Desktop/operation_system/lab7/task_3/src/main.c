/**
* <u><b>Завдання #3 ( Очікування на завершення потоку )</b></u>
<br>Напишіть програму, що створює два нових потоків виконання.<br>
<i>Перший потік:</i><br>
 <ol>
  <li>Нащадків виводить в стандартний потік виведення задану кількість разів заданий рядок
(аналогічно рядкам з Завдання No1). </li>
</ol>
<i>Другий потік:</i>
<ol>
 <li>Генерує задану кількість цілих псевдовипадкових чисел із заданого діапазону</li>
 <li>Виводить їх у стандартний потік виведення у вигляді, аналогічному виведенню інформації в першому потоці — нащадку.</li>
</ol>
<i>Принцип роботи:</i>
Якщо під час
роботи даного потоку буде отримано задане псевдовипадкове число, то потік передчасно
завершує свою роботу з видачею відповідного повідомлення. Основний, батьківський потік,
чекає на закінчення роботи всіх потоків — нащадків і після їх завершення виводить в
стандартний потік виведення своє повідомлення.
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

const long MIN = 1;
const long MAX = 10000;
const long AMOUNT = 1000;
long WAIT_FOR_SECONDS = 3;

typedef struct toThread {
    char* str;
}ThreadData;

// function for thread 1
// TODO Чому важливо виводити через stderr. У файлі це пояснюється це буферизацією
void * thread2(void *args) {
    int numFromArgs = *(int*) args;
    int num;
    int i;
    for (i = 0; i < AMOUNT; i++) {
        num = (rand() % (MAX - MIN + 1)) + MIN;
        if (num = numFromArgs) {
            fprintf(stderr, "Ми знайшли номер!\n");
            pthread_exit(NULL);
        }
    }
}

void * thread1(void *args) {
    ThreadData data = * (ThreadData *) args;
    for (int i = 0; i < 10; i++) {
        fprintf(stderr, "Child Thread. Iteration: %d. Message: %s\n", (i+1), data.str);
        sleep(WAIT_FOR_SECONDS);
    }

    return NULL;
}

void check_if_error(int result) {
    if (result != 0) {
        fprintf(stderr, "Error during thread creation\n");
        exit(1);
    }
}

int main() {
    // передається в 1 потік
    ThreadData threadData;
    threadData.str = "ha-ha-ha-ha";
    // передається в 2 потік
    int selected_num = 1000;


    pthread_t pthread1, pthread2;

    check_if_error(pthread_create(&pthread1, NULL, &thread1, &threadData));
    if (pthread_join(pthread1, NULL) != 0) {
        fprintf(stderr, "Join error\n");
        return 1;
    }

    check_if_error(pthread_create(&pthread2, NULL, &thread2, &selected_num));
    if (pthread_join(pthread2, NULL) != 0) {
        fprintf(stderr, "Join error\n");
        return 1;
    }

    fprintf(stderr, "Хорошо, основной поток завершен.!\n");




    return 0;
}