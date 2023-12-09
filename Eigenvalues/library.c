#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "headers.h"

double *inputMatrix(FILE* fin, int n) {
    double* a = (double*)malloc(sizeof(double) * n*n);
    double number;

    for (int i = 0; i < n*n; i++) {
        if (fscanf(fin, "%lf", &number) != 1) {
            printf("Error while reading a matrix from file.\n");
            exit(1);
            return 0;
        }
        a[i] = number;
    }

    return a;
}

void printMatrix(int n, int m, double *a) {
    int b = (n > m ? m : n);
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < b; j++) {
            printf("%10.3g ", a[i * n + j]);
        }
        printf("\n");
    }
}