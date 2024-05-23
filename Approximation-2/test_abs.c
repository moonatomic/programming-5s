#include <math.h>
#include <stdio.h>
#include "headers.h"

void test_abs(int amount, int prec, double left, double right) {
    FILE *fout;
    fout = fopen("abs1.dat", "w");

    double norm;
    double errmax;

    double points[amount];
    double points_approx[amount*prec];
    double values[amount];
    double values_approx[amount*prec];

    double xi[amount+1];
    double v[amount+1];

    for (int i = 0; i < amount; i++) {
        points[i] = left + 2*right*i / amount;
        values[i] = fabs(points[i]);
    }

    xi_fill(points, xi, amount);
    v_fill(v, points, values, xi, amount);

    norm = 0;

    for (int i = 0; i < amount; i++) {
        for (int j = 0; j < prec; j++) {
            points_approx[i*prec + j] = left + 2*right*(i*prec + j) / (amount*prec);
        }
        for (int j = 0; j < prec; j++) {
            values_approx[i*amount + j] = general(points_approx[i*prec + j], v[i], v[i+1], values[i], points[i], xi[i], xi[i+1]);
        }
    }
    for (int k = 0; k < amount*prec; k++) {
        fprintf(fout, "%lf %lf %lf %lf\n", points_approx[k], values_approx[k], fabs(points_approx[k]), 3*fabs(fabs(points_approx[k]) - values_approx[k]));
        if (fabs(fabs(points_approx[k]) - values_approx[k]) > norm) {
            norm = fabs(fabs(points_approx[k]) - values_approx[k]);
            errmax = points_approx[k];
        }
    }
    fclose(fout);
    printf("Error for abs(x) at %d points:\n%lf at x=%lf\n", amount, norm, errmax);
}