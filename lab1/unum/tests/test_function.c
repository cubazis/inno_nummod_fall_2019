#include <check.h>
#include <stdlib.h>
#include "posit.h"

START_TEST(test_posit_function)
{
  ck_assert_int_eq(42, test_function());
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
  TCase *tcase = tcase_create("test_function");
  tcase_add_checked_fixture(tcase, setup, teardown);
  tcase_set_timeout(tcase, 60);

  tcase_add_test(tcase, test_posit_function);


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
