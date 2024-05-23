#include "headers.h"
#include <math.h>
#include <stdio.h>
#define EPS 1e-16

double fsin(double x, double y) {
    return sin(3*x) + sin(2*y); 
}

double ffabs(double x, double y) {
    return sqrt(fabs(x) + fabs(y));
}

void test_sin(double left, double right, int amount, int prec) {
    double x[amount+1];
    double y[amount+1];
    double f[6];
    double value_approx;

    double norm;

    Point p, p1, p2, p3;

    FILE *fout;
    fout = fopen("sin.dat", "w");

    norm = 0;

    for (int i = 0; i < amount+1; i++) {
        x[i] = left + i * (right-left)/amount;
        y[i] = x[i];
    }

    for (int i = 0; i < amount*prec+1; i++) {
        p.x = left + i * (right-left)/(amount*prec);
        for (int j = 0; j < amount*prec+1; j++) {
            p.y = left + j * (right-left)/(amount*prec);
            triangulation(p, x, y, amount, &p1, &p2, &p3);
            f[0] = fsin(p1.x, p1.y);
            f[1] = fsin(p2.x, p2.y);
            f[2] = fsin(p3.x, p3.y);
            f[3] = fsin(avg(p1,p2).x, avg(p1,p2).y);
            f[4] = fsin(avg(p2,p3).x, avg(p2,p3).y);
            f[5] = fsin(avg(p1,p3).x, avg(p1,p3).y);

            value_approx = value(p, p1, p2, p3, f);

            fprintf(fout, "%lf %lf %lf\n", p.x, p.y, value_approx);
            if (fabs(value_approx - fsin(p.x, p.y)) > norm) {
                norm = fabs(value_approx - fsin(p.x, p.y));
            }
        }
    }
    fclose(fout);
    printf("Error for sin(3*x) + cos(2*y) at %d points:\n%10.3e\n", amount, norm);
}

void test_abs(double left, double right, int amount, int prec) {
    double x[amount+1];
    double y[amount+1];
    double f[6];
    double value_approx;

    double norm;

    Point p, p1, p2, p3;

    FILE *fout;
    fout = fopen("abs.dat", "w");

    norm = 0;

    for (int i = 0; i < amount+1; i++) {
        x[i] = left + i * (right-left)/amount;
        y[i] = x[i];
    }

    for (int i = 0; i < amount*prec+1; i++) {
        p.x = left + i * (right-left)/(amount*prec);
        for (int j = 0; j < amount*prec+1; j++) {
            p.y = left + j * (right-left)/(amount*prec);
            triangulation(p, x, y, amount, &p1, &p2, &p3);
            f[0] = ffabs(p1.x, p1.y);
            f[1] = ffabs(p2.x, p2.y);
            f[2] = ffabs(p3.x, p3.y);
            f[3] = ffabs(avg(p1,p2).x, avg(p1,p2).y);
            f[4] = ffabs(avg(p2,p3).x, avg(p2,p3).y);
            f[5] = ffabs(avg(p1,p3).x, avg(p1,p3).y);

            value_approx = value(p, p1, p2, p3, f);

            fprintf(fout, "%lf %lf %lf\n", p.x, p.y, fabs(value_approx-ffabs(p.x, p.y)));
            if (fabs(value_approx - ffabs(p.x, p.y)) > norm) {
                norm = fabs(value_approx - ffabs(p.x, p.y));
            }
        }
    }
    fclose(fout);
    printf("Error for sqrt(abs(x) + abs(y)) at %d points:\n%10.3e\n", amount, norm);
}
