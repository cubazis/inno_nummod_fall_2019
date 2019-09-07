#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>

#define BUF_SIZE 33

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

int bs[5] = {5, 8, 12, 15, 20};
posit res;
posit eps;

char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

void init_v1(struct vector *v, double a)
{
    posit p = double2posit(10.0);

    v->x1 = posit_pow(p, a);

    v->x2 = double2posit(1223.0);

    v->x3 = posit_pow(p, a - 1);

    v->x4 = posit_pow(p, a - 2);

    v->x5 = double2posit(3.0);

    v->x6 = negate(posit_pow(p, a - 5));
}

void init_v2(struct vector *v, double b)
{
    posit p = double2posit(10.0);

    v->x1 = posit_pow(p, b);

    v->x2 = double2posit(2.0);

    v->x3 = negate(posit_pow(p, b + 1));

    v->x4 = posit_pow(p, b);

    v->x5 = double2posit(2111.0);

    v->x6 = posit_pow(p, b + 3);
}

posit dot(struct vector *v1, struct vector *v2)
{
    posit result = multiplication(v1->x1, v2->x1);
    result = sum(result, multiplication(v1->x2, v2->x2));
    result = sum(result, multiplication(v1->x3, v2->x3));
    result = sum(result, multiplication(v1->x4, v2->x4));
    result = sum(result, multiplication(v1->x5, v2->x5));
    result = sum(result, multiplication(v1->x6, v2->x6));

    return result;
}

void print_vec_bin(struct vector *v)
{
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';

    int2bin(v->x1.data, buffer, BUF_SIZE - 1);
    printf("x1 %s\n", buffer);

    int2bin(v->x2.data, buffer, BUF_SIZE - 1);
    printf("x2 %s\n", buffer);

    int2bin(v->x3.data, buffer, BUF_SIZE - 1);
    printf("x3 %s\n", buffer);

    int2bin(v->x4.data, buffer, BUF_SIZE - 1);
    printf("x4 %s\n", buffer);

    int2bin(v->x5.data, buffer, BUF_SIZE - 1);
    printf("x5 %s\n", buffer);

    int2bin(v->x6.data, buffer, BUF_SIZE - 1);
    printf("x6 %s\n", buffer);
}

void print_vec(struct vector *v) {
    printf("x1: %f\n", posit2float(v->x1));
    printf("x2: %f\n", posit2float(v->x2));
    printf("x3: %f\n", posit2float(v->x3));
    printf("x4: %f\n", posit2float(v->x4));
    printf("x5: %f\n", posit2float(v->x5));
    printf("x6: %f\n", posit2float(v->x6));
}

START_TEST(test_a5)
{
    res = double2posit(8779.0);
    eps = double2posit(10e-8);

	printf("test_posit_a5\n");
	init_v1(x, 5);
    for (int i = 0; i < 1; ++i) {
        init_v2(y, bs[i]);

        posit dot_res = dot(x, y);

        printf("%lf\n", posit2float(dot_res));

        int a = lessthan(posit_abs(diff(res, dot_res)), eps);

        ck_assert(isgreaterequal((double)a, 1.0));
    }
}
END_TEST

START_TEST(test_a10)
{
	printf("test_double_a10\n");
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

int main (void) {
	int number_failed;
	Suite *suite = str_suite();
	SRunner *runner = srunner_create(suite);
	srunner_run_all(runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return number_failed;
}