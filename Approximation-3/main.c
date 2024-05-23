#include "headers.h"
#define AMOUNT 50
#define PREC 10
#define LEFT -1
#define RIGHT 1

int main(void) {
    test_sin(LEFT, RIGHT, AMOUNT, PREC);
    test_abs(LEFT, RIGHT, AMOUNT, PREC);
    return 0;
}