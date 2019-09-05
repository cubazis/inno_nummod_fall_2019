#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long double res = 1.644725755214774951;
double eps = 10e-18;
int limit = 4801;

START_TEST(test_sum)
{
	printf("test_sum\n");
	long double sum = 0;
	for (int i = 1; i < limit; ++i) {
		sum += 1/powl((long double)i, 2);
	}
	printf("%.18Lf\n", fabsl(sum - res));
	ck_assert(fabsl(sum - res) < eps);
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
	TCase *tcase = tcase_create("test_sum");
	tcase_add_checked_fixture(tcase, setup, teardown);
	tcase_set_timeout(tcase, 60);

	tcase_add_test(tcase, test_sum);


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