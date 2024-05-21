#include <math.h>
#include <stdio.h>
#include "headers.h"

void test_sin_outl(int amount, int prec, double left, double right) {
    FILE *fsin;
    fsin = fopen("sin2.dat", "w");

    double norm;
    double errmax;

    double points[amount];
    double points_approx[amount*prec];
    double values[amount];
    double values_approx[amount*prec];
    double di[amount];

    for (int i = 0; i < amount; i++) {
        points[i] = left + 2*right*i / amount;
        values[i] = sin(points[i]);
    }
    values[amount/2] = 0.01;

    d_Akima_fill(points, values, amount, di);
    norm = 0;

    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < prec; j++) {
            points_approx[i*prec + j] = left + 2*right*(i*prec + j) / (amount*prec);
        }
        for (int j = 0; j < prec; j++) {
            if (i < 2) {
                values_approx[i*prec + j] = interpol_pol(points_approx[i*prec + j], points[0], points[1], points[2], values[0], values[1], values[2], di[2]);
            } else if (i > amount-3) {
                values_approx[i*prec + j] = interpol_pol(points_approx[i*prec + j], points[amount-3], points[amount-2], points[amount-1], values[amount-3], values[amount-2], values[amount-1], di[amount-3]);
            } else {
                values_approx[i*prec + j] = general_pol(points_approx[i*prec + j], points[i], points[i+1], values[i], values[i+1], di[i], di[i+1]);
            }
        }
    }
    for (int k = 2*prec+1; k < amount*prec-3*prec; k++) {
        fprintf(fsin, "%lf %lf %lf\n", points_approx[k], values_approx[k], sin(points_approx[k]));
        if (fabs(sin(points_approx[k]) - values_approx[k]) > norm) {
            norm = fabs(sin(points_approx[k]) - values_approx[k]);
            errmax = points_approx[k];
        }
    }
    fclose(fsin);
    printf("Error for sin(x) with outline at 0 at %d points:\n%1.3g at x=%lf\n", amount, norm, errmax);
}