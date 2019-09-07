//
// Created by cubazis on 18.08.2019.
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

struct posit32 {
    bool sign;
    int32_t exponent;
    uint32_t fraction;
};

double to_double(struct posit32 p);

struct posit32 to_posit32(double f);

struct posit32 sum(struct posit32 p1, struct posit32 p2);

struct posit32 add(struct posit32 p1, struct posit32 p2);

struct posit32 subtract(struct posit32 p1, struct posit32 p2);

struct posit32 multiply(struct posit32 p1, struct posit32 p2);

struct posit32 dot_product(size_t dim, struct posit32 *v1, struct posit32 *v2);