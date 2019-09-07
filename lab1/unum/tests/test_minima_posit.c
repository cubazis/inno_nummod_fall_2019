#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>


int limit = 10000000;

posit half_divide_method(posit a, posit b, posit stop, posit (*f)(posit x), int iterations)
{
	posit e = double2posit(0.5);
	posit x = posit_divide(sum(a,b),2);
	int i = 0;
	while(beq(f(x), stop) && (i < iterations)) {
		x = posit_divide(sum(a,b),2);
		posit f1 = f(diff(x,e));
		posit f2 = f(sum(x,e));
		if (lessthan(f1, f2)) {
			b = x;
		} else {
			a = x;
		}
		i += 1;
	}
	return posit_divide(sum(a,b),2);
}

posit parabola(posit x)
{
  return posit_pow(diff(x, double2posit(1.333333333333)), 2);
}

START_TEST(test_minima_posit)
{
//	double minima = half_divide_method(-2, 4.65,  0.0001, parabola, 10000000);
//	printf("%.16lf\n", minima);
//	printf("%.16lf\n", res);
//	printf("%.16lf\n", fabs(res - minima));
//	ck_assert(fabs(res - minima) < eps);
	posit minima = half_divide_method(double2posit(-2), double2posit(4.65), double2posit(0.0001), parabola,2000000);
	posit res = double2posit(1.3333333333333333);
	posit diff_res = posit_abs(diff(res, minima));
	posit eps = double2posit(10e-8);
	printf("%.16f\n", posit2float(minima));
	printf("%.16f\n",  posit2float(res));
	printf("%.16f\n",  posit2float(diff_res));
	ck_assert(lessthan(diff_res, eps));
}
END_TEST

void setup(void)
{

}

void teardown(void)
{

}

Suite* str_suite (void)
{
	Suite *suite = suite_create("posit");
	TCase *tcase = tcase_create("test_minima_posit");
	tcase_add_checked_fixture(tcase, setup, teardown);
	tcase_set_timeout(tcase, 60);

	tcase_add_test(tcase, test_minima_posit);


	suite_add_tcase(suite, tcase);
	return suite;
}

int main (void) {
	int number_failed;
	Suite *suite = str_suite();
	SRunner *runner = srunner_create(suite);
	srunner_run_all(runner, CK_NORMAL);
	//srunner_set_fork_status(runner, CK_NOFORK);
	number_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return number_failed;
}