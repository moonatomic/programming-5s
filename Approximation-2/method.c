#include "headers.h"
#include <stdio.h>
#define EPS 1e-16

// Модуль вещественного числа
double fabs(double a) {
    return a > 0 ? a : -1*a;
}

// Разделенная разность
double diff(double fi, double fj, double xi, double xj) {
    return (fj - fi) / (xj - xi);
}

int solve_system(double *a, double *b, double *x) {
    double max = 0;
    int maxi;
    double swp;
    double coeff;

    for (int i = 0; i < 4; i++) {
        max = fabs(a[i*4 + i]);
        maxi = i;
        for (int k = i; k < 4; k++) { // Поиск максимального по модулю элемента столбца
            if (fabs(a[k*4 + i]) > max) {
                maxi = k;
                max = fabs(a[k*4 + i]);
                for (int j = i; j < 4; j++) {
                    swp = a[i*4 + j];
                    a[i*4 + j] = a[k*4 + j];
                    a[k*4 + j] = swp;
                }
                swp = b[i];
                b[i] = b[k];
                b[k] = swp;
            }
        }
        if (fabs(a[i]) < EPS) {
            return 1;
        } else {
            coeff = a[i*4 + i];
            for (int j = i; j < 4; j++) {
                a[i*4 + j] = a[i*4 + j] / coeff;
            }
            b[i] = b[i] / coeff;
            for (int k = i+1; k < 4; k++) {
                coeff = a[k*4 + i];
                for (int j = i; j < 4; j++) {
                    a[k*4 + j] = a[k*4 + j] - coeff*a[i*4 + j];
                }
                b[k] = b[k] - coeff*b[i];
            }
        }
        for (int i = 0; i < 4; i++) {
            x[i] = b[i];
        }
    }

    return 0;
}

// Заполнение массива d_i-ых по методу Акимы
// В массиве нет первых двух и последних двух d_i-ых
void d_Akima_fill(double *x, double *f, int n, double *d) {
    double wip1, wim1;
    for (int i = 2; i < n-2; i++) {
        wim1 = fabs(diff(f[i-1], f[i], x[i-1], x[i]) - diff(f[i-2], f[i-1], x[i-2], x[i-1]));
        wip1 = fabs(diff(f[i+1], f[i+2], x[i+1], x[i+2]) - diff(f[i], f[i+1], x[i], x[i+1]));
        if (wim1 < EPS && wip1 < EPS) {
            d[i] = ((x[i+1] - x[i])*diff(f[i-1], f[i], x[i-1], x[i]) + (x[i]-x[i-1])*diff(f[i], f[i+1], x[i], x[i+1])) / (x[i+1] - x[i-1]);
        } else {
            d[i] = (wip1*diff(f[i-1], f[i], x[i-1], x[i]) + wim1*diff(f[i], f[i+1], x[i], x[i+1])) / (wip1 + wim1);
        }
    }
}

// Значения приближающего многочлена вне крайних отрезков
// Общая схема интерполяции
double general_pol(double x, double xi, double xip1, double fi, double fip1, double di, double dip1) {
    double a1, a2, a3, a4;
    a1 = fi;
    a2 = di;
    a3 = (diff(fi, fip1, xi, xip1) - di) / (xip1 - xi);
    a4 = (di + dip1 - 2*diff(fi, fip1, xi, xip1)) / ((xip1 - xi)*(xip1 - xi));

    return a1 + a2*(x-xi) + a3*(x-xi)*(x-xi) + a4*(x-xi)*(x-xi)*(x-xip1);
}

// Значения приближающего многочлена на крайних отрезках
// Интерполяционный многочлен 3 степени
double interpol_pol(double x, double x0, double x1, double x2, double f0, double f1, double f2, double d) {
    double a[16];
    double b[4];
    double ai[4];
    int err;

    // СЛАУ для нахождения интерполяционного многочлена
    a[0] = 1; a[1] = x0; a[2] = x0*x0; a[3] = x0*x0*x0;
    a[4] = 1; a[5] = x1; a[6] = x1*x1; a[7] = x1*x1*x1;
    a[8] = 1; a[9] = x2; a[10] = x2*x2; a[11] = x2*x2*x2;
    a[12] = 0; a[13] = 1; a[14] = 2*x2; a[15] = 3*x2*x2;
    b[0] = f0;
    b[1] = f1;
    b[2] = f2;
    b[3] = d;

    err = solve_system(a, b, ai);
    if (err) {
        printf("Error while interpolating in %lf\n", x);
        return 0;
    } else {
        return ai[0] + ai[1]*x + ai[2]*x*x + ai[3]*x*x*x;
    }
}