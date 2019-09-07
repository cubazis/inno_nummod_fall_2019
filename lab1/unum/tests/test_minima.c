#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double res = 1.3333333333333333;
int limit = 10000000;
double eps = 10e-8;

double half_divide_method(double a, double  b, double stop, double (*f)(double x), int iterations)
{
  double e = 0.5;
  double x = (a + b) / 2.0;
  int i = 0;
  while ((f(x) >= stop) && (i < iterations))
  {
    x = (a + b) / 2.0;
    double f1 = f(x - e);
    double f2 = f(x + e);
    if(f1 < f2){
      b = x;
    }
    else{
      a = x;
    }
    i += 1;
  }
  return (a + b) / 2.0;
}

double parabola(double x)
{
  return pow((x - 4.0/3.0), 2);
}

START_TEST(test_minima)
{
  double minima = half_divide_method(-2, 4.65,  0.0001, parabola, 10000000);
  printf("%.16lf\n", minima);
  printf("%.16lf\n", res);
  printf("%.16lf\n", fabs(res - minima));
  ck_assert(fabs(res - minima) < eps);
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
  TCase *tcase = tcase_create("test_minima");
  tcase_add_checked_fixture(tcase, setup, teardown);
  tcase_set_timeout(tcase, 60);

  tcase_add_test(tcase, test_minima);


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
