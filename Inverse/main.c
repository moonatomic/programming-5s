#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "headers.h"

int main(int argc, char **argv) {
    double timer; // Секундомер работы алгоритма
    double *a; // Входная матрица
    double *x; // Матрица-ответ
    double err; // Норма невязки
    FILE *fin; // Файл с входной матрицей

    clock_t start; // Начало и конец отсчета времени
    clock_t stop;

    int n, m, k; // Входные параметры
    char *filename = 0;

    if (argc < 4 || argc > 5) {
        printf("Wrong input.\n");
        return -1;
    } else {
        n = atoi(argv[1]); // Получение аргументов из терминала
        m = atoi(argv[2]);
        k = atoi(argv[3]);
        if (k == 0) {
            filename = argv[4];
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
                    a[i*n + j] = (i > j ? i : j) + 1;
                } else if (k == 3) {
                    a[i*n + j] = abs(i - j);
                } else if (k == 4) {
                    a[i*n + j] = 1.0 / (i+j + 1);
                }
            }
        }
    }

    printf("Matrix A initialized:\n"); // Вывод инициализированной матрицы
    printMatrix(n, m, a);
    printf("\n");

    x = (double*)malloc(sizeof(double) * n*n); // Создание единичной матрицы, которая станет ответом
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                x[i*n + j] = 1.0;
            } else {
                x[i*n + j] = 0.0;
            }
        }
    }

    timer = 0.0;
    start = clock(); // Засекаем время
    invertMatrix(n, a, x); // Обращаем матрицу
    stop = clock();
    timer = (double)(stop-start) / CLOCKS_PER_SEC;

    printf("Matrix A inverted:\n"); // Вывод обращенной матрицы
    printMatrix(n, m, x);
    printf("\nThis took %lf sec.\n", timer);

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
                    a[i*n + j] = (i > j ? i : j) + 1;
                } else if (k == 3) {
                    a[i*n + j] = abs(i - j);
                } else if (k == 4) {
                    a[i*n + j] = 1.0 / (i+j + 1);
                }
            }
        }
    }

    err = solutionError(n, a, x); // Вычисляем (Фробениусову) норму невязки и выводим
    printf("Calculations error: %10.3g\n", err);

    free(a);
    free(x);
    return 0;
}
