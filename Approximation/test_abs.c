#include <math.h>
#include <stdio.h>
#include "headers.h"

void test_abs(int amount, int prec, double left, double right) {
    FILE *fout;
    fout = fopen("abs.dat", "w");

    double norm;
    double errmax;

    double points[amount];
    double points_approx[amount*prec];
    double values[amount];
    double values_approx[amount*prec];
    double di[amount];

    for (int i = 0; i < amount; i++) {
        points[i] = left + 2*right*i / amount;
        values[i] = fabs(points[i]);
    }

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
        fprintf(fout, "%lf %lf %lf %1.3g\n", points_approx[k], values_approx[k], fabs(points_approx[k]), 10*fabs(fabs(points_approx[k]) - values_approx[k]));
        if (fabs(fabs(points_approx[k]) - values_approx[k]) > norm) {
            norm = fabs(fabs(points_approx[k]) - values_approx[k]);
            errmax = points_approx[k];
        }
    }
    fclose(fout);
    printf("Error for abs(x) at %d points:\n%1.3g at x=%lf\n", amount, norm, errmax);
}