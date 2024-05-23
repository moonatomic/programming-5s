#include <stdio.h>
#include <math.h>
#include "headers.h"

#define LEFT -M_PI // Левая и правая границы
#define RIGHT M_PI

#define AMOUNT 500 // Количество отрезков разбиения (и точек интерполяции)
#define PREC 1000 // Точность -- количество точек приближения на каждом отрезке

int main(void) {
    test_sin(AMOUNT, PREC, LEFT, RIGHT);
    test_sin_outl(AMOUNT, PREC, LEFT, RIGHT);
    test_abs(AMOUNT, PREC, LEFT, RIGHT);
    return 0;
}