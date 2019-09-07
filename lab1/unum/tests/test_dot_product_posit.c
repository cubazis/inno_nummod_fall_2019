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

struct vector* x;
struct vector* y;

void init_v1(struct vector *v, int a)
{
    v->x1 = posit_pow(double2posit(10), a);
    v->x2 = double2posit(1223.0);
    v->x3 = posit_pow(double2posit(10),a-1);
    v->x4 = posit_pow(double2posit(10),a-2);
    v->x5 = double2posit(3.0);
    v->x6 = negate(posit_pow(double2posit(10), a-5));
}

void init_v2(struct vector *v, int b)
{
    v->x1 = posit_pow(double2posit(10), b);
    v->x2 = double2posit(2.0);
    v->x3 = negate(posit_pow(double2posit(10),b+1));
    v->x4 = posit_pow(double2posit(10),b);
    v->x5 = double2posit(2111.0);
    v->x6 = posit_pow(double2posit(10), b+3);
}

posit dot(struct vector *v1, struct vector *v2)
{
    posit x1m = multiplication(v1->x1, v2->x1);
    posit x2m = multiplication(v1->x2, v2->x2);
    posit x3m = multiplication(v1->x3, v2->x3);
    posit x4m = multiplication(v1->x4, v2->x4);
    posit x5m = multiplication(v1->x5, v2->x5);
    posit x6m = multiplication(v1->x6, v2->x6);
    posit result = sum(sum(sum(sum(sum(x1m, x2m),x3m),x4m),x5m),x6m);
    return result;
}

void print_vec(struct vector *v)
{
    printf("{%f, %f, %f, %f, %f, %f}\n", posit2float(v->x1), posit2float(v->x2), posit2float(v->x3), posit2float(v->x4), posit2float(v->x5), posit2float(v->x6));
}

START_TEST(test_a5)
{
    double bs[5] = {5, 8, 12, 15, 20};
    posit res = double2posit(8779.0);
    posit eps = double2posit(10e-8);
    printf("test_posit_a5\n");
    init_v1(x, 5);
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);
        printf("%f\n", posit2float(dot(x, y)));
        ck_assert(lessthan(posit_abs(diff(res, dot(x,y))), eps));
    }
}
END_TEST

START_TEST(test_a10)
{
    double bs[5] = {5, 8, 12, 15, 20};
    posit res = double2posit(8779.0);
    posit eps = double2posit(10e-8);
    printf("test_posit_a10\n");
    init_v1(x, 10);
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);
        printf("%f\n", posit2float(dot(x, y)));
        ck_assert(lessthan(posit_abs(diff(res, dot(x,y))), eps));
    }
}
END_TEST

void setup(void)
{
	x = calloc(1, sizeof(struct vector));
	y = calloc(1, sizeof(struct vector));
}

void teardown(void)
{
	free(x);
	free(y);
}

Suite* str_suite (void)
{
	Suite *suite = suite_create("posit");
	TCase *tcase = tcase_create("test_dot_product_posit");
	tcase_add_checked_fixture(tcase, setup, teardown);
	tcase_set_timeout(tcase, 60);

	tcase_add_test(tcase, test_a5);
	tcase_add_test(tcase, test_a10);


	suite_add_tcase(suite, tcase);
	return suite;
}
//
int main (void) {
	int number_failed;
	Suite *suite = str_suite();
	SRunner *runner = srunner_create(suite);
	srunner_run_all(runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return number_failed;
}