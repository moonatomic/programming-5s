#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

double *inputMatrix(FILE* fin, int n); // Чтение матрицы из файла
double solutionError(int n, double *a, double *x); // Вычисление нормы невязки
int invertMatrix(int n, double *a, double *x); // Обращение матрицы
void printMatrix(int n, int m, double *a); // Вывод первых m строк и столбцов матрицы 
void *f_thr(void *arg); // Рутина потока