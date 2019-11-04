#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int __n;
static double *__a;
static double *__b;

#define __A(i,j) __a[(i)*10+(j)]

int blackbox_size();

void blackbox_init() {
    scanf("%d", &__n);
    
    __a = (double*) malloc(100 * sizeof(double));
    int row, col;
    unsigned int tmp = 40;
    for (row = 0; row < 10; row++) {
        for (col = 0; col <= row; col++) {
            __A(row, col) = __A(col, row) = ((int)tmp) / 2147483647. + (row == col) * 5;
            tmp = (1664525*tmp + 1013904223) % 0xffffffff;
        }
    }
}

int blackbox_size() {
    return 10;
}

void blackbox_mult(const double *x, double *out) {
    for (size_t row = 0; row < 10; row++) {
        out[row] = 0.0;
        for (size_t col = 0; col < 10; col++) {
            out[row] += __A(row, col) * x[col] * 1.1;
        }
    }
}

void blackbox_rhs(double *b) {
    int elem;
    unsigned int tmp = 43;
    for (elem = 0; elem < 10; elem++) {
        b[elem] = ((int)tmp) / 2147483647. + 2;
        tmp = (1664525*tmp + 1013904223) % 0xffffffff;
    }
}

void blackbox_submit(double *solution) {
    double *tmp1 = (double *) malloc(10 * sizeof(double));
    double *tmp2 = (double *) malloc(10 * sizeof(double));
    blackbox_mult(solution, tmp1);
    blackbox_rhs(tmp2);
    double residual = 0;
    for (int row = 0; row < blackbox_size(); ++row) {
        double diff = tmp1[row] - tmp2[row];
        residual += diff*diff;
    }
    residual = sqrt(residual);
    free(__a);
    free(tmp1);
    free(tmp2);
    printf("%.12f\n", residual);
}

#undef __A
