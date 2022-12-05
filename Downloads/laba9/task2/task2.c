#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>

#include "my_threads.h"
#include "my_threads.c"
#include "integr.h"
#include "integr.c"

double fun(double x) {
    return 4.0 - x*x;
}

int main(int argc, char ** argv) {
    int thread_amount = 5;
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                thread_amount = atoi(optarg);
                break;
        }
    }

    double res;
    printf("Integral Calculation\n");
    printf("Threaded Application\n");
    {
        TARG arg = {thread_amount, 0.0, 2.0, 1.0e-6, fun};
        res = Thread_NIntegrate(&arg);
    }
    printf("\tRes: %g\n", res);

    return EXIT_SUCCESS;
}
