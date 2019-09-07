#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct vector {
  double x1;
  double x2;
  double x3;
  double x4;
  double x5;
  double x6;
};

struct vector* x;
struct vector* y;

double bs[5] = {5, 8, 12, 15, 20};
double res = 8779;
double eps = 10e-8;

void init_v1(struct vector *v, double a)
{
  v->x1 = pow(10, a);
  v->x2 = 1223;
  v->x3 = pow(10, a-1);
  v->x4 = pow(10, a-2);
  v->x5 = 3;
  v->x6 = -pow(10, a-5);
}

void init_v2(struct vector *v, double b)
{
  v->x1 = pow(10, b);
  v->x2 = 2;
  v->x3 = -pow(10, b+1);
  v->x4 = pow(10, b);
  v->x5 = 2111;
  v->x6 = pow(10, b+3);
}

double dot(struct vector *v1, struct vector *v2)
{
  return v1->x1*v2->x1+v1->x2*v2->x2+v1->x3*v2->x3+v1->x4*v2->x4+v1->x5*v2->x5+v1->x6*v2->x6;
}

void print_vec(struct vector *v)
{
  printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", v->x1, v->x2, v->x3, v->x4, v->x5, v->x6);
}

START_TEST(test_a5)
{
  printf("test_double_a5\n");
  init_v1(x, 5);
  for (int i = 0; i < 5; ++i) {
    init_v2(y, bs[i]);
    printf("%lf\n", dot(x, y));
    ck_assert(fabs(res - dot(x,y)) < eps);
  }
}
END_TEST

START_TEST(test_a10)
{
  printf("test_double_a10\n");
  init_v1(x, 10);
  for (int i = 0; i < 5; ++i) {
    init_v2(y, bs[i]);
    printf("%lf\n", dot(x, y));
    ck_assert(fabs(res - dot(x,y)) < eps);
  }
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
  TCase *tcase = tcase_create("test_dot_product");
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
