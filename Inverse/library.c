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

double solutionError(int n, double *a, double *x) {
    double ans;
    double elem;

    ans = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            elem = 0.0;
            for (int k = 0; k < n; k++) {
                elem += a[i*n + k] * x[k*n + j];
            }
            if (i == j) {
                elem -= 1;
            }
            ans += elem * elem;
        }
    }
    return sqrt(ans);
}

int invertMatrix(int n, double *a, double *x) {
    int leader;
    double swp;
    double coeff;
    double norm;
    double max;

    for (int i = 0; i < n; i++) {
        max = a[i*n + i];
        leader = i; 
        for (int j = i+1; j < n; j++) { //Ищем лидера по столбцу
            if (max < fabs(a[j*n + i])) {
                max = fabs(a[j*n + i]);
                leader = j;
            }
        }
        if (i != leader) { // Переставляем строку с лидером наверх
            for (int j = 0; j < n; j++) {
                swp = a[i*n + j];
                a[i*n + j] = a[leader*n + j];
                a[leader*n + j] = swp;
            } // То же самое во второй матрице
            for (int j = 0; j < n; j++) {
                swp = x[i*n + j];
                x[i*n + j] = x[leader*n + j];
                x[leader*n + j] = swp;
            }
        }
        norm = 1.0 / a[i*n + i];
        for (int j = 0; j < n; j++) { // Нормируем верхнюю строку
            a[i*n + j] *= norm;
        }
        for (int j = 0; j < n; j++) {
            x[i*n + j] *= norm;
        }

        for (int j = 0; j < i; j++) { // Вычитаем строку из всех что выше
            coeff = a[j*n + i];
            for (int k = i; k < n; k++)
                a[j*n + k] -= a[i*n + k] * coeff;

            for (int k = 0; k < n; k++)
                x[j*n + k] -= x[i*n + k] * coeff;
        }

        for (int j = i+1; j < n; ++j) { // И из всех что ниже
            coeff = a[j*n + i];
            for (int k = i; k < n; ++k)
                a[j*n + k] -= a[i*n + k] * coeff;

            for (int k = 0; k < n; ++k)
                x[j*n + k] -= x[i*n + k] * coeff;
        }
    }
    return 0;
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
