#ifndef INTEGR_H_
#define INTEGR_H_

double integrate(double (*f)(double x), double a, double b, unsigned long n);
double NIntegrate(double (*f)(double x), double a, double b, double eps);

#endif