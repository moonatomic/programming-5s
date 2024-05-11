#include <stdio.h>
#include <math.h>
#include "headers.h"

#define LEFT -M_PI // Левая и правая границы
#define RIGHT M_PI

#define AMOUNT 2000 // Количество отрезков разбиения (и точек интерполяции)
#define PREC 100 // Точность -- количество точек приближения на каждом отрезке

int main(void) {
    FILE *fsin, *fab, *foutl;
    fsin = fopen("sin1.dat", "w");

    double norm;
    double errmax;

    double points[AMOUNT];
    double points_approx[AMOUNT*PREC];
    double values[AMOUNT];
    double values_approx[AMOUNT * PREC];
    double di[AMOUNT];

    for (int i = 0; i < AMOUNT; i++) {
        points[i] = LEFT + 2*RIGHT*i / AMOUNT;
        values[i] = sin(points[i]);
    }

    d_Akima_fill(points, values, AMOUNT, di);
    norm = 0;

    for (int i = 0; i < AMOUNT; i++) {
        for (int j = 0; j < PREC; j++) {
            points_approx[i*PREC + j] = LEFT + 2*RIGHT*(i*PREC + j) / (AMOUNT*PREC);
        }
        for (int j = 0; j < PREC; j++) {
            if (i < 2) {
                values_approx[i*PREC + j] = interpol_pol(points_approx[i*PREC + j], points[0], points[1], points[2], values[0], values[1], values[2], di[2]);
            } else if (i > AMOUNT-3) {
                values_approx[i*PREC + j] = interpol_pol(points_approx[i*PREC + j], points[AMOUNT-3], points[AMOUNT-2], points[AMOUNT-1], values[AMOUNT-3], values[AMOUNT-2], values[AMOUNT-1], di[AMOUNT-3]);
            } else {
                values_approx[i*PREC + j] = general_pol(points_approx[i*PREC + j], points[i], points[i+1], values[i], values[i+1], di[i], di[i+1]);
            }
        }
    }
    for (int k = 2*PREC+1; k < AMOUNT*PREC-3*PREC-1; k++) {
        fprintf(fsin, "%lf %lf %lf\n", points_approx[k], values_approx[k], sin(points_approx[k]));
        if (fabs(sin(points_approx[k]) - values_approx[k]) > norm) {
            norm = fabs(sin(points_approx[k]) - values_approx[k]);
            errmax = points_approx[k];
        }
    }
    fclose(fsin);
    printf("Error for sin(x) at %d points:\n%1.3g at x=%lf\n", AMOUNT, norm, errmax);
}