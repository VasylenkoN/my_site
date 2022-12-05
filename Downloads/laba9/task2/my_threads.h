#ifndef MY_THREADS_
#define MY_THREADS_

typedef struct arg_str {
	int n; // кількість прямокутників/потоків
	double a, b, eps; // a - лівий проміжок, b - правий проміжок, eps -
	double (*f)(double); // ф-ція
} TARG;

double Thread_NIntegrate(const TARG * arg);

#endif