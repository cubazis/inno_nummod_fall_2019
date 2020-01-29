#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>

struct vector {
	struct posit32 x1;
    struct posit32 x2;
    struct posit32 x3;
    struct posit32 x4;
    struct posit32 x5;
    struct posit32 x6;
};

struct vector* x;
struct vector* y;

double bs[5] = {5, 8, 12, 15, 20};
double res = 8779;
double eps = 10e-8;

void init_v1(struct vector *v, double a)
{
	v->x1 = to_posit32(pow(10, a));
	v->x2 = to_posit32(1223);
	v->x3 = to_posit32(pow(10, a-1));
	v->x4 = to_posit32(pow(10, a-2));
	v->x5 = to_posit32(3);
	v->x6 = to_posit32(-pow(10, a-5));
}

void init_v2(struct vector *v, double b)
{
	v->x1 = to_posit32(pow(10, b));
	v->x2 = to_posit32(2);
	v->x3 = to_posit32(-pow(10, b+1));
	v->x4 = to_posit32(pow(10, b));
	v->x5 = to_posit32(2111);
	v->x6 = to_posit32(pow(10, b+3));
}

struct posit32 dot(struct vector *v1, struct vector *v2)
{
	struct posit32 *vec1, *vec2;
	vec1 = calloc(6, sizeof(struct posit32));
	vec1[0] = v1->x1;
	vec1[1] = v1->x2;
	vec1[2] = v1->x3;
	vec1[3] = v1->x4;
	vec1[4] = v1->x5;
	vec1[5] = v1->x6;

	vec2 = calloc(6, sizeof(struct posit32));
	vec2[0] = v2->x1;
	vec2[1] = v2->x2;
	vec2[2] = v2->x3;
	vec2[3] = v2->x4;
	vec2[4] = v2->x5;
	vec2[5] = v2->x6;

	return dot_product(6, vec1, vec2);
}

void print_vec(struct vector *v)
{
	double x1 = to_double(v->x1);
	double x2 = to_double(v->x2);
	double x3 = to_double(v->x3);
	double x4 = to_double(v->x4);
	double x5 = to_double(v->x5);
	double x6 = to_double(v->x6);
	printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", x1, x2, x3, x4, x5, x6);
}

START_TEST(test_a5)
{
	printf("test_double_a5\n");
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