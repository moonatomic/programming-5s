#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "headers.h"

#define MAXITER 100000

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

void printVector(int n, double *x) {
    for (int i = 0; i < n; i++) {
        printf("%10.3e ", x[i]);
    }
    printf("\n");
}

void reflections(int n, double *a) {
	int i;
	int j;
	int k;
	double tmp1;
	double tmp2;

	for (i = 0; i < n-2; i++) {
		tmp1 = 0.0;
		for (j = i+2; j < n; j++) {
			tmp1 += a[j*n + i] * a[j*n + i];
        }
		tmp2 = sqrt(a[(i+1)*n + i] * a[(i+1)*n + i] + tmp1);

		if (tmp2 < 1e-100) {
			a[(i+1)*n + i] = 0.0;
			a[(i+2)*n + i] = 0.0;
			continue;
		}

		if (tmp1 < 1e-100) {
			a[(i+2)*n + i] = 0.0;
			continue;
		}

		a[(i+1)*n + i] -= tmp2;

		tmp1 = 1.0 / sqrt(a[(i+1)*n + i] * a[(i+1)*n + i] + tmp1);
		for (j = i + 1; j < n; ++j) {
			a[j*n + i] *= tmp1;
        }

		for (j = i + 1; j < n; j++) {
			tmp1 = 0.0;
			for (k = i + 1; k < n; k++) {
				tmp1 += a[k*n + i] * a[k*n + j];
            }

			tmp1 *= 2.0;
			for (k = i + 1; k < n; k++) {
				a[k*n + j] -= tmp1 * a[k*n + i];
            }
		}

		for (j = 0; j < n; ++j) {
			tmp1 = 0.0;
			for (k = i + 1; k < n; k++) {
				tmp1 += a[k*n + i] * a[j*n + k];
            }

			tmp1 *= 2.0;
			for (k = i + 1; k < n; k++) {
				a[j*n + k] -= tmp1 * a[k*n + i];
            }
		}

		a[(i+1)*n + i] = tmp2;
		for (j = i + 2; j < n; j++) {
			a[j*n + i] = 0.0;
        }
	}
}

void LR(int n, double *a, int d, double eps) {
    for (int i = 1; i < d; i++) {
        a[i*n + i-1] /= a[(i-1)*n + i-1];

        for (int j = i; j < d; j++) {
            a[i*n + j] -= a[i*n + i-1] * a[(i-1)*n + j];
        }
    }
}

void RL(int n, double *a, int d, double eps) {
    LR(n, a, d, eps);
    for (int i = 0; i < d; i++) {
        if (i != 0) {
            a[i*n + i-1] *= a[i*n + i];
        }
        for (int j = i; j < d-1; j++) {
            a[i*n + j] += a[i*n + j+1] * a[(j+1)*n + j];
        }
    }
}

double maxRowNorm(int n, double *a) {
    double sum;
    double norm = 0.0;

    for (int i = 0; i < n; i++) {
        sum = 0.0;
        for (int j = 0; j < n; j++) {
            sum += fabs(a[i*n + j]);
        }
        if (sum > norm) {
            norm = sum;
        }
    }

    return norm;
}

void shift(int n, double *a, double s, int d) {
    for (int i = 0; i < d; i++) {
        a[i*n + i] -= s;
    }
}

int zeroDiag(int n, double *a, int d, double eps) {
    for (int i = 0; i < d-1; i++) {
        if (fabs(a[i*n + i]) < eps) {
            return 0;
        }
    }
    return 1;
}

int equation(double a, double b, double c, double *x, double eps) {
    double D = b*b - 4*a*c;
    if (D < 0) {
        return 1;
    }
    if (fabs(D) < eps) {
        x[0] = -b / (2*a);
        x[1] = -b / (2*a);
        return 0;
    } else {
        double x1 = (-b - sqrt(D)) / (2*a);
        double x2 = (-b + sqrt(D)) / (2*a);
        if (fabs(x1) > fabs(x2)) {
            x[0] = x1;
            x[1] = x2;
            return 0;
        } else {
            x[0] = x2;
            x[1] = x1;
            return 0;
        }
    }
}

int LRAlgorithm(int n, double *a, double *x, int d, double eps, double norm) {
    if (d == 2) {
        return equation(1.0, -a[0] - a[n+1], a[0] * a[n+1] - a[1] * a[n], x, eps);
    }

    int count;
    int count2;
    double s = a[(d-1)*n + d-1];

    count = 0;

    while ((fabs(a[(d-1)*n + d-2]) > eps * norm) && count != MAXITER) {
        shift(n, a, s, d);
        count2 = 0;
        while (zeroDiag(n, a, d, eps) == 0 && count2 != MAXITER) {
            shift(n, a, s * 1e-6, d);
            s += s * 1e-6;
            count2++;
        }
        if (count2 == MAXITER) {
            printf("BRUH\n");
            return 1;
        }

        RL(n, a, d, eps);

        shift(n, a, -s, d);
        s = a[(d-1)*n + d-1];
        count++;
    }

    if (count == MAXITER) {
        return 1;
    }
    x[d-1] = s;
    return LRAlgorithm(n, a, x, d-1, eps, norm);
}

int LREigenvalues(int n, double *a, double *x, double eps) {
    return LRAlgorithm(n, a, x, n, eps, maxRowNorm(n, a));
}

double solutionError(int n, double *a, double *x) {
    double ans;
    ans = 0;

    for (int i = 0; i < n; i++) {
        ans += a[i*n + i] - x[i];
    }

    return fabs(ans);
}