#include <iostream>
#include <cmath>


#include "../cmake-build-debug/googletest-src/googletest/include/gtest/gtest.h"
#include "../posit.h"

struct vector {
    Posit x1;
    Posit x2;
    Posit x3;
    Posit x4;
    Posit x5;
    Posit x6;
};

struct vector *x;
struct vector *y;

double bs[5] = {5, 8, 12, 15, 20};
double res = 8779;
double eps = 10e-8;

void init_v1(struct vector *v, double a) {
    v->x1 = Posit(pow(10, a));
    v->x2 = Posit(1223);
    v->x3 = Posit(pow(10, a - 1));
    v->x4 = Posit(pow(10, a - 2));
    v->x5 = Posit(3);
    v->x6 = Posit(-pow(10, a - 5));
}

void init_v2(struct vector *v, double b) {
    v->x1 = Posit(pow(10, b));
    v->x2 = Posit(2);
    v->x3 = Posit(-pow(10, b + 1));
    v->x4 = Posit(pow(10, b));
    v->x5 = Posit(2111);
    v->x6 = Posit(pow(10, b + 3));
}

Posit dot(struct vector *v1, struct vector *v2) {
    return v1->x1 * v2->x1 + v1->x3 * v2->x3 + v1->x4 * v2->x4 + v1->x6 * v2->x6 + v1->x5 * v2->x5 + v1->x2 * v2->x2;
}

void print_vec(struct vector *v) {
    printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", v->x1.toDouble(), v->x2.toDouble(),
           v->x3.toDouble(), v->x4.toDouble(), v->x5.toDouble(), v->x6.toDouble());
}

void setup(void) {
    x = static_cast<vector *>(calloc(1, sizeof(struct vector)));
    y = static_cast<vector *>(calloc(1, sizeof(struct vector)));
}


TEST(test_a5_case, test_a5) {
    setup();
    printf("test_double_a5\n");
    init_v1(x, 5);
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);
        printf("%lf\n", dot(x, y).toDouble());
        EXPECT_EQ(fabs(res - dot(x, y).toDouble()) < eps, true);
    }
}

TEST(test_a10_case, test_a10) {
    setup();
    printf("test_double_a10\n");
    init_v1(x, 10);
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);
        print_vec(x);
        print_vec(y);
        printf("%lf\n", dot(x, y).toDouble());
        EXPECT_EQ(fabs(res - dot(x, y).toDouble()) < eps, true);
    }
}

