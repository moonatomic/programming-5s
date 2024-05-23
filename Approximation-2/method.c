#include "headers.h"
#include <stdio.h>
#define EPS 1e-16

// Модуль вещественного числа
double fabs(double a) {
    return a > 0 ? a : -1*a;
}

// Кси_i-ые
void xi_fill(double *x, double *xi, int n) {
    xi[0] = x[0] - (x[1]-x[0])/2;
    for (int i = 1; i < n; i++) {
        xi[i] = (x[i-1] + x[i])/2;
    }
    xi[n] = x[n-1] + (x[n-1]-x[n-2])/2;
}

// Заполнение массива v_i-ых
void v_fill(double *vi, double *x, double *f, double *xi, int n) {
    int err = 0;

    double a[(n+1)*(n+1)];
    double b[n+1];

    b[0] = f[0]*(1.0/(x[0] - xi[0]) + 1.0/(xi[1] - x[0]));
    b[n] = f[n-1]*(1.0/(x[n-1] - xi[n-1]) + 1.0/(xi[n] - x[n-1]));
    for (int i = 1; i < n; i++) {
        b[i] = (1.0/(x[i-1] - xi[i-1]) + 1.0/(xi[i]-x[i-1]))*f[i-1] + (1.0/(x[i] - xi[i]) + 1.0/(xi[i+1]-x[i]))*f[i];
    }


    for (int i = 0; i < (n+1)*(n+1); i++) {
        a[i] = 0;
    }

    for (int i = 1; i < n; i++) {
        a[i*(n+1) + i - 1] = 1.0/(x[i-1] - xi[i-1]) - 1.0/(xi[i] - xi[i-1]);
        a[i*(n+1) + i] = 1.0/(xi[i] - x[i-1]) + 1.0/(xi[i] - xi[i-1]) + 1.0/(x[i] - xi[i]) + 1.0/(xi[i+1] - xi[i]);
        a[i*(n+1) + i + 1] = 1.0/(xi[i+1] - x[i]) - 1.0/(xi[i+1] - xi[i]); 
    }

    a[0] = 1.0/(x[0] - xi[0]);
    a[1] = 1.0/(xi[1] - x[0]);
    a[(n+1)*(n+1) - 1] = 1.0/(xi[n] - x[n-1]);
    a[(n+1)*(n+1) - 2] = 1.0/(x[n-1] - xi[n-1]);

    if (solve_system(a, b, vi, n+1)) {
        printf("Error while interpolating!\n");
    }
}

int solve_system(double *a, double *b, double *x, int n) {
    double coeff;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j != i) {
                if (fabs(a[j*n + i]) > EPS) {
                    coeff = a[j*n + i] / a[i*n + i];
                    for (int k = i; k < n; k++) {
                        a[j*n + k] = a[j*n + k] - coeff*a[i*n + k];
                    }
                    b[j] = b[j] - coeff * b[i];
                }
            }
        }
        coeff = a[i*n + i];
        b[i] = b[i]/coeff;
        for (int j = 0; j < n; j++) {
            a[i*n + j] = a[i*n + j]/coeff;
        }
    }
    for (int i = 0; i < n; i++) {
        x[i] = b[i];
    }
    /*
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", a[i*n + j]);
        }
        printf("| %lf\n", b[i]);
    }
    */
    return 0;
}

// Общая схема интерполяции
double general(double x, double vi, double vip1, double fi, double xi, double xii, double xiip1) {
    double a1, a2, a3;
    a1 = vi;
    a2 = (fi-vi)/(xi-xii);
    a3 = (1.0/(xiip1 - xii))*((vip1 - fi)/(xiip1 - xi) - (fi - vi)/(xi - xii));

    return a1 + a2*(x-xii) + a3*(x-xii)*(x-xi);
}