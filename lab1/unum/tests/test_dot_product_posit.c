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

posit bs[5];
posit res;
posit eps;

void init_v1(struct vector *v, posit a)
{
	v->x1 = p_pow(make_posit(10), a);
	v->x2 = make_posit(1223);
	v->x3 = p_pow(make_posit(10), sub(a, make_posit(1)));
	v->x4 = p_pow(make_posit(10), sub(a, make_posit(2)));
	v->x5 = make_posit(3);
	v->x6 = get_neg_p(p_pow(make_posit(10), sub(a, make_posit(5))));
}

void init_v2(struct vector *v, posit b)
{
	v->x1 = p_pow(make_posit(10), b);
	v->x2 = make_posit(2);
	v->x3 = get_neg_p(p_pow(make_posit(10), add(b, make_posit(1))));
	v->x4 = p_pow(make_posit(10), b);
	v->x5 = make_posit(2111);
	v->x6 = make_posit(pow(10, pack_double(unpack_posit(b)) + 3));
}

posit dot(struct vector *v1, struct vector *v2)
{
	posit r1 = add(mult(v1->x1, v2->x1), mult(v1->x3, v2->x3));
	posit r2 = add(mult(v1->x4, v2->x4), mult(v1->x6, v2->x6));
	posit r3 = add(mult(v1->x2, v2->x2), mult(v1->x5, v2->x5));
	return add(add(r1, r2), r3);
}

void print_vec(struct vector *v)
{
	double x1 = pack_double(unpack_posit(v->x1));
	double x2 = pack_double(unpack_posit(v->x2));
	double x3 = pack_double(unpack_posit(v->x3));
	double x4 = pack_double(unpack_posit(v->x4));
	double x5 = pack_double(unpack_posit(v->x5));
	double x6 = pack_double(unpack_posit(v->x6));
	printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", x1, x2, x3, x4, x5, x6);
}

START_TEST(test_a5)
{
	printf("test_double_a5\n");
	init_v1(x, make_posit(5));
	for (int i = 0; i < 5; ++i) {
		init_v2(y, bs[i]);
		posit dot_pr = dot(x, y);
		ck_assert(is_less(p_abs(sub(res, dot_pr)), eps));
	}
}
END_TEST

START_TEST(test_a10)
{
	printf("test_double_a10\n");
	init_v1(x, make_posit(10));
	for (int i = 0; i < 5; ++i) {
		init_v2(y, bs[i]);
		posit dot_pr = dot(x, y);
		ck_assert(is_less(p_abs(sub(res, dot_pr)), eps));
	}
}
END_TEST

void setup(void)
{
	x = calloc(1, sizeof(struct vector));
	y = calloc(1, sizeof(struct vector));
	bs[0] = make_posit(5);
	bs[1] = make_posit(8);
	bs[2] = make_posit(12);
	bs[3] = make_posit(15);
	bs[4] = make_posit(20);

	res = make_posit(8779);
	eps = make_posit(10e-8);
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