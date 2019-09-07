#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "posit.h"
#include "posit_conversion.h"
#include "posit_ops.h"

struct vector {
    posit_unpack_t x1;
    posit_unpack_t x2;
    posit_unpack_t x3;
    posit_unpack_t x4;
    posit_unpack_t x5;
    posit_unpack_t x6;
};

struct vector *x;
struct vector *y;

posit_unpack_t bs[5];
posit_unpack_t res;
posit_unpack_t eps;

void init_v1(struct vector *v, posit_unpack_t a) {
    v->x1 = posit_power(from_double(10), a);
    v->x2 = from_double(1223);
    v->x3 = posit_power(from_double(10), posit_sub(a, from_double(1)));
    v->x4 = posit_power(from_double(10), posit_sub(a, from_double(2)));
    v->x5 = from_double(3);
    v->x6 = posit_neg(posit_power(from_double(10), posit_sub(a, from_double(5))));
}

void init_v2(struct vector *v, posit_unpack_t b) {
    v->x1 = posit_power(from_double(10), b);
    v->x2 = from_double(2);
    v->x3 = posit_neg(posit_power(from_double(10), posit_add(b, from_double(1))));
    v->x4 = posit_power(from_double(10), b);
    v->x5 = from_double(2111);
    v->x6 = posit_power(from_double(10), posit_add(b, from_double(3)));
}

posit dot(struct vector *v1, struct vector *v2) {
    posit_unpack_t arr1[6] = {v1->x1, v1->x2, v1->x3, v1->x4, v1->x5, v1->x6};
    posit_unpack_t arr2[6] = {v2->x1, v2->x2, v2->x3, v2->x4, v2->x5, v2->x6};

    return posit_dot_product(arr1, arr2, 6);
}

void print_vec(struct vector *v) {
    printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n",
           to_double(v->x1), to_double(v->x2), to_double(v->x3), to_double(v->x4), to_double(v->x5), to_double(v->x6));
}

START_TEST(test_a5) {
        printf("test_posit_a5\n");
        init_v1(x, from_double(5));
        for (int i = 0; i < 5; ++i) {
            init_v2(y, bs[i]);
            printf("%lf\n", to_double(dot(x, y)));
            ck_assert(posit_is_less(posit_abs(posit_sub(res, dot(x, y))), eps));
        }
}

END_TEST

START_TEST(test_a10) {
    printf("test_posit_a5\n");
    init_v1(x, from_double(10));
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);
        printf("%lf\n", to_double(dot(x, y)));
        ck_assert(posit_is_less(posit_abs(posit_sub(res, dot(x, y))), eps));
        ck_assert(0);
    }
}

END_TEST

void setup(void) {
    x = calloc(1, sizeof(struct vector));
    y = calloc(1, sizeof(struct vector));

    double bs_double[5] = {5, 8, 12, 15, 20};
    for (int i = 0; i < 5; ++i) {
        bs[i] = from_double(bs_double[i]);
    }

    res = from_double(8779);
    eps = from_double(10e-8);
}

void teardown(void) {
    free(x);
    free(y);
}

Suite *str_suite(void) {
    Suite *suite = suite_create("posit");
    TCase *tcase = tcase_create("test_dot_product_posit");
    tcase_add_checked_fixture(tcase, setup, teardown);
    tcase_set_timeout(tcase, 60);

    tcase_add_test(tcase, test_a5);
    tcase_add_test(tcase, test_a10);


    suite_add_tcase(suite, tcase);
    return suite;
}

int main(void) {
    int number_failed;
    Suite *suite = str_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed;
}