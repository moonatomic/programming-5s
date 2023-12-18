#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "headers.h"

int main(int argc, char **argv) {
    double timer; // Секундомер работы алгоритма
    double *a; // Входная матрица
    double *x; // Список собственных значений
    double err; // Норма невязки (первый вариант)

    FILE *fin; // Файл с входной матрицей

    clock_t start; // Начало и конец отсчета времени
    clock_t stop;

    int n, m, k; // Входные параметры
    double e;
    char *filename = 0;

    if (argc < 5 || argc > 6) {
        printf("Wrong input.\n");
        return -1;
    } else {
        n = atoi(argv[1]); // Получение аргументов из терминала
        m = atoi(argv[2]);
        e = atof(argv[3]);
        k = atoi(argv[4]);
        if (k == 0) {
            filename = argv[5];
        }
    }

    if (k == 0) {
        if (filename == 0) {
            printf("No filename.\n");
            return -1;
        } else {
            fin = fopen(filename, "r");
            a = inputMatrix(fin, n); // Читаем матрицу из файла
            fclose(fin);
        }
    } else { // Или инициализируем по формуле
        a = (double*)malloc(sizeof(double) * n*n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (k == 1) {
                    a[i*n + j] = n - (i > j ? i : j);
                } else if (k == 2) {
                    if (i == j) {
                        a[i*n + j] = 2;
                    } else if (abs(i-j) == 1) {
                        a[i*n + j] = 1;
                    } else {
                        a[i*n + j] = 0;
                    }
                } else if (k == 3) {
                    if (i == j && i < n-1) {
                        a[i*n + j] = 1;
                    } else if (j == n-1) {
                        a[i*n + j] = i+1;
                    } else if (i == n-1) {
                        a[i*n + j] = j+1;
                    }
                } else if (k == 4) {
                    a[i*n + j] = 1.0 / (i+j + 1);
                }
            }
        }
    }

    printf("Matrix A initialized:\n"); // Вывод инициализированной матрицы
    printMatrix(n, m, a);
    printf("\n");

    reflections(n, a);

    x = (double*)malloc(n*sizeof(double));

    timer = 0.0;
    start = clock();
    if (LREigenvalues(n, a, x, e) != 0) {
        printf("Error: unable to run algorithm.\n");
        free(a);
        free(x);
        return -1;
    }
    stop = clock();
    timer = (double)(stop-start) / CLOCKS_PER_SEC;

    printf("Found eigenvalues:\n");
    printVector(n, x);
    printf("This took: %lf sec.\n", timer);
    
    err = solutionError(n, a, x);
    printf("Error in calculations: %10.3g.\n", err);


    free(a);
    free(x);
    return 0;
}