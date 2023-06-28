/**
* <b>Завдання №4 ( Повернення потоком результату ) </b><br>
 * Напишіть програму, що створює два нових потоку виконання:
<ul>
    <li>Перший потік: обчислює задану кількість перших чисел Каталана</li>
    <li>Другий потік: обчислює задану кількість перших простих чисел</li>
    <li>Головний потік: повертає масиви з кожного потока і виводить результати розрахунків в стандартний потік виведення</li>
</ul>
<u><i>Доповнення</i></u><br>
Спробувати різні способи повернення потоком обчислених значень
<ul>
    <li>Глобальна змінна</li>
    <li>Повернення через результуючу змінну потокової функції</li>
    <li>Через аргумент потокової функції</li>
</ul>
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

//#include "arraylist.c"

#define bigint unsigned long long int

void* thread1(void *args);

void* thread2(void *args);

bigint catalan_number(int n);

void check_if_error(int result);

int main() {
    int amount = 5;
    int array1[amount];

    int* array =(int* ) malloc(amount * sizeof(int));
    if (array == NULL) {
        printf("MEMORY_ALLOCATION_ERROR");
        return 1;
    }

    pthread_t pthread1, pthread2;
    check_if_error(pthread_create(&pthread1, NULL, &thread1, &amount));
    if (pthread_join(pthread1, (void*) &array) != 0) {
        fprintf(stderr, "Join error\n");
        return 1;
    }
    printf("\nsimple nums: ");
    for (int i = 0; i < amount; i++) {
        printf("%d ", array[i]);
    }
//--------------------------------------------------------------------
    bigint* arrayBig =(bigint* ) malloc(amount * sizeof(bigint));
    if (arrayBig == NULL) {
        printf("MEMORY_ALLOCATION_ERROR");
        return 1;
    }

    check_if_error(pthread_create(&pthread2, NULL, &thread2, &amount));
    if (pthread_join(pthread2, (void*) &arrayBig) != 0) {
        fprintf(stderr, "Join error\n");
        return 1;
    }
    printf("\ncatalan nums: ");
    for (int i = 0; i < amount; i++) {
        printf("%llu ", arrayBig[i]);
    }
    printf("\n");
    return 0;
}

void check_if_error(int result) {
    if (result != 0) {
        fprintf(stderr, "Error during thread creation\n");
        exit(1);
    }
}

void* thread1(void *args) {

    int i = 2;
    int count = 1;
    int flag, j, index;
    int n = *(int *) args;

    int arr[n];
    int* array =(int* ) malloc(n * sizeof(int));
    if (array == NULL) {
        printf("MEMORY_ALLOCATION_ERROR");
        exit(1);
    }

    /* Generating prime numbers */
    while (count <= n) {
        flag = 0;

        for (j = 2; j <= i / 2; j++)
            if (i % j == 0) {
                flag = 1;
                break;
            }

        if (flag == 0) {
//            printf("%d\t", i);
//            arr[count - 1] = i;
            array[count - 1] = i;
            count++;
        }
        i++;
    }

    return (void*) array;
}

void* thread2(void *args) {
    int n = * (int* ) args;
//    bigint arr[n];

    bigint* array =(bigint* ) malloc(n * sizeof(bigint));
    if (array == NULL) {
        printf("MEMORY_ALLOCATION_ERROR");
        exit(1);
    }


    for (int i = 0; i < n; ++i) {
//        arr[i] = catalan_number(i);
        array[i] = catalan_number(i);
//        printf("%llu", arr[i]);
    }
    return (void*) array;
}

bigint catalan_number(int n) {
    bigint b = 1;

    // calculating n!
    for (int i = 1; i <= n; i++) {
        b = b * i;
    }

    // calculating n! * n!
    b = b * b;

    bigint d = 1;

    // calculating (2n)!
    for (int i = 1; i <= 2 * n; i++) {
        d = d * i;
    }

    // calculating (2n)! / (n! * n!)
    bigint ans = d / b;

    // calculating (2n)! / ((n! * n!) * (n+1))
    ans = ans / (n + 1);
    return ans;

}