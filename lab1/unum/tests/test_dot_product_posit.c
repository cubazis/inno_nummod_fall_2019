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

posit bs[5] = {{0x52000000}, {0x58000000}, {0x5C000000}, {0x5F000000}, {0x61000000}}; //5, 8, 12, 15, 20
posit res = {0x79125800}; //8779
posit eps = {0x012B5FCA}; //10e-8
 
void init_v1(struct vector *v, posit a)
{
    posit posit_10 = {0x5A000000}; //10
	v->x1 = power(posit_10, a);
    posit posit_1223 = {0x74638000}; //1223
	v->x2 = posit_1223; 
	v->x3 = power(posit_10, sub(a, one())); //a - 1
    posit posit_2 = {0x48000000}; //2
	v->x4 = power(posit_10, sub(a, posit_2)); //a - 2
    posit posit_3 = {0x4C000000}; //3
	v->x5 = posit_3;
    posit posit_5 = {0x52000000}; 
	v->x6 = neg(power(posit_10, sub(a, posit_5))); //a - 5
}

void init_v2(struct vector *v, posit b)
{
    posit posit_10 = {0x5A000000}; //10
	v->x1 = power(posit_10, b);
    posit posit_2 = {0x48000000}; //2
    v->x2 = posit_2;
	v->x3 = neg(power(posit_10, add(b, one())));
	v->x4 = power(posit_10, b);
    posit posit_2111 = {0x760FC000}; //2111
	v->x5 = posit_2111;
    posit posit_3 = {0x4C000000}; //3
	v->x6 = power(posit_10, add(b, posit_3));
}

posit dot(struct vector *v1, struct vector *v2)
{
    posit v1x1_v2x1 = mul(v1->x1, v2->x1);
    posit v1x2_v2x2 = mul(v1->x2, v2->x2);
    posit v1x3_v2x3 = mul(v1->x3, v2->x3);
    posit v1x4_v2x4 = mul(v1->x4, v2->x4);
    posit v1x5_v2x5 = mul(v1->x5, v2->x5);
    posit v1x6_v2x6 = mul(v1->x6, v2->x6);

    posit preresult_1 = add(v1x1_v2x1, v1x2_v2x2);
    posit preresult_2 = add(v1x5_v2x5, v1x6_v2x6);
    preresult_1 = add(preresult_1, v1x3_v2x3);
    preresult_2 = add(preresult_2, v1x4_v2x4);
    
    return add(preresult_1, preresult_2);
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
