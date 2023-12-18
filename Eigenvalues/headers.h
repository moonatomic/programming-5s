#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double *inputMatrix(FILE* fin, int n);
void printMatrix(int n, int m, double *a);
void reflections(int n, double *a);
void printVector(int n, double *x);
void LR(int n, double *a, int d, double eps);
void RL(int n, double *a, int d, double eps);
void shift(int n, double *a, double s, int d);
double maxRowNorm(int n, double *a);
int zeroDiag(int n, double *a, int d, double eps);
int equation(double a, double b, double c, double *x, double eps);
int LRAlgorithm(int n, double *a, double *x, int d, double eps, double norm);
int LREigenvalues(int n, double *a, double *x, double eps);
double solutionError(int n, double *a, double *x);