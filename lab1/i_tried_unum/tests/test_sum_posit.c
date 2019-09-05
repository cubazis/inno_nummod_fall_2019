#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>

int limit = 4801;

/** 1.644725755214775 */
posit res;
/** 10e-15 */
posit eps;

START_TEST(test_sum_posit)
{
	printf("test_sum_posit\n");

	for (int i = 0; i < limit; ++i) {

	}

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
	TCase *tcase = tcase_create("test_sum_posit");
	tcase_add_checked_fixture(tcase, setup, teardown);
	tcase_set_timeout(tcase, 60);

	tcase_add_test(tcase, test_sum_posit);


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