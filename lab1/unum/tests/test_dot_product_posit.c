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
posit res;
posit eps;

void init_v1(struct vector *v, posit a) {

}

void init_v2(struct vector *v, posit b) {

}

posit dot(struct vector *v1, struct vector *v2) {

}

void print_vec(struct vector *v) {

}

START_TEST(test_a5)
        {
                printf("test_double_a5\n");
        }

END_TEST

START_TEST(test_a10) {
    printf("test_double_a10\n");
}

END_TEST

void setup(void) {
    x = calloc(1, sizeof(struct vector));
    y = calloc(1, sizeof(struct vector));
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