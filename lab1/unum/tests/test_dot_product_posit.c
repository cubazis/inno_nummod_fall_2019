#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>

struct vector {
    posit x1;
    posit x2;
    posit x3;
    posit x4;
    posit x5;
    posit x6;
};

struct vector *x;
struct vector *y;

posit bs[5];
posit result;
posit eps;

void init_v1(struct vector *v, posit a) {
    v->x1 = power(from_double(10), a);
    v->x2 = from_double(1223);
    v->x3 = power(from_double(10), subtract(a, from_double(1)));
    v->x4 = power(from_double(10), subtract(a, from_double(2)));
    v->x5 = from_double(3);
    v->x6 = get_neg_p(power(from_double(10), subtract(a, from_double(5))));
}

void init_v2(struct vector *v, posit b) {
    v->x1 = power(from_double(10), b);
    v->x2 = from_double(2);
    v->x3 = get_neg_p(power(from_double(10), add(b, from_double(1))));
    v->x4 = power(from_double(10), b);
    v->x5 = from_double(2111);
    v->x6 = from_double(pow(10, get_double(b) + 3));
}

posit dot(struct vector *v1, struct vector *v2) {
    posit r1 = add(multiply(v1->x1, v2->x1), multiply(v1->x3, v2->x3));
    posit r2 = add(multiply(v1->x4, v2->x4), multiply(v1->x6, v2->x6));
    posit r3 = add(multiply(v1->x2, v2->x2), multiply(v1->x5, v2->x5));
    return add(add(r1, r2), r3);
}

void print_vec(struct vector *v) {
    double x1 = get_double(v->x1);
    double x2 = get_double(v->x2);
    double x3 = get_double(v->x3);
    double x4 = get_double(v->x4);
    double x5 = get_double(v->x5);
    double x6 = get_double(v->x6);
    printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", x1, x2, x3, x4, x5, x6);
}

START_TEST(test_a5)
    {
        printf("test_double_a5\n");
        init_v1(x, from_double(5));
        for (int i = 0; i < 5; ++i) {
            init_v2(y, bs[i]);
            posit dot_pr = dot(x, y);
            ck_assert(is_less(posit_abs(subtract(result, dot_pr)), eps));
        }
    }
END_TEST

START_TEST(test_a10)
    {
        printf("test_double_a10\n");
        init_v1(x, from_double(10));
        for (int i = 0; i < 5; ++i) {
            init_v2(y, bs[i]);
            posit dot_pr = dot(x, y);
            ck_assert(is_less(posit_abs(subtract(result, dot_pr)), eps));
        }
    }
END_TEST

void setup(void) {
    x = calloc(1, sizeof(struct vector));
    y = calloc(1, sizeof(struct vector));
    bs[0] = from_double(5);
    bs[1] = from_double(8);
    bs[2] = from_double(12);
    bs[3] = from_double(15);
    bs[4] = from_double(20);

    result = from_double(8779);
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
