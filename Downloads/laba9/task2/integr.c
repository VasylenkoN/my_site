#include "integr.h"
#include <stdio.h>
#include <math.h>

double integrate(double (*f)(double x), double a, double b, unsigned long n) {
	double res, h;
	unsigned long i;
	h = (b - a) / n;
	res = 0;
	for (i = 0; i < n; i++) {
		res += f(a + (i + 0.5)*h);
	}
	res *= h;
	return res;
}

double NIntegrate(double (*f)(double x), double a, double b, double eps) {
	double i0, i1, error;
	unsigned long n = 10;
	
	i0 = integrate(f, a, b, n);
	do {
		n *= 2;
		i1 = integrate(f, a, b, n);
		error = fabs(i0 - i1);
		i0 = i1;
	} while(error >= eps);
	
	return i1;
}