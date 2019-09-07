#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

struct posit {
    bool sign;
    int32_t exponent;
    uint32_t fraction;
};

struct posit add_posits(struct posit left, struct posit right);

struct posit subtract_posits(struct posit left, struct posit right);

struct posit summa_posits(struct posit left, struct posit right);

struct posit multiply_posits(struct posit left, struct posit right);

struct posit dot_product(size_t dim, struct posit *left, struct posit *right);

double to_double(struct posit number);

struct posit to_posit(double number);