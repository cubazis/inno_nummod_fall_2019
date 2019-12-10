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

posit bs[5] = {{0x52000000}, {0x58000000}, {0x5C000000}, {0x5F000000}, {0x61000000}};
posit res = {0x79125800};
posit eps = {0x012B5FCA};

void init_v1(struct vector *v, posit a)
{
  posit ten = {0x5A000000}, x_1223 = {0x74638000};
  posit two = {0x48000000}, three = {0x4C000000};
  v->x1 = power(ten, a);
  v->x2 = x_1223;
  v->x3 = power(ten, sub(a, one()));
  v->x4 = power(ten, sub(a, two));
  v->x5 = three;
  v->x6 = neg(power(ten, sub(a, three)));
}

void init_v2(struct vector *v, posit b)
{
  posit ten = {0x5A000000}, x_2111 = {0x760FC000};
  posit two = {0x48000000}, three = {0x4C000000};
  v->x1 = power(ten, b);
  v->x2 = two;
  v->x3 = neg(power(ten, add(b, one())));
  v->x4 = power(ten, b);
  v->x5 = x_2111;
  v->x6 = power(ten, add(b, three));
}

posit dot(struct vector *v1, struct vector *v2)
{
  posit mul1 = mul(v1->x1, v2->x1);
  posit mul2 = mul(v1->x2, v2->x2);
  posit mul3 = mul(v1->x3, v2->x3);
  posit mul4 = mul(v1->x4, v2->x4);
  posit mul5 = mul(v1->x5, v2->x5);
  posit mul6 = mul(v1->x6, v2->x6);

  posit add1 = add(mul1, mul2);
  add1 = add(add1, mul3);
  add1 = add(add1, mul4);
  add1 = add(add1, mul5);
  add1 = add(add1, mul6);
  return add1;
}

void print_vec(struct vector *v)
{
  print(v->x1);
  print(v->x2);
  print(v->x3);
  print(v->x4);
  print(v->x5);
  print(v->x6);
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
