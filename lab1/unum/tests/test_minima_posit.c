#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>


int limit = 10000000;

/** 1.3333333333333333 */
posit res;

/** 10e-8 */
posit eps;

double half_divide_method(posit a, posit b, posit stop, posit (*f)(posit x), int iterations)
{

}

posit parabola(posit x)
{

}

START_TEST(test_minima_posit)
{

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
