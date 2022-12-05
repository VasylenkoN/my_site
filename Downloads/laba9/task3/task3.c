#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>

#include "task.h"
#include "tarray.h"
#include "task.c"
#include "tarray.c"
#include "threads.c"

int main(int argc, char ** argv) {
    int array_size = 7;

    int opt;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                array_size = atoi(optarg);
                break;
        }
    }

    printf("Task3. Read/Write Lock\n");
    {
        TPARAM init;
        TARRAY arr;
        tarray_init(&arr, array_size);
        init.arr = &arr;
        init.num_readers = 6;
        init.num_writers = 3;
        init.num_reps = 10;
        task_solution(&init);
        tarray_dest(&arr);
    }

    return EXIT_SUCCESS;
}
