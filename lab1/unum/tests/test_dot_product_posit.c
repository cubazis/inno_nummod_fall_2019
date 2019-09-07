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
    v->x1 = power(fromDouble(10), a);
    v->x2 = fromDouble(1223);
    v->x3 = power(fromDouble(10), sub(a, fromDouble(1)));
    v->x4 = power(fromDouble(10), sub(a, fromDouble(2)));
    v->x5 = fromDouble(3);
    v->x6 = negate(power(fromDouble(10), sub(a, fromDouble(5))));
}

void init_v2(struct vector *v, posit b)
{
    v->x1 = power(fromDouble(10), b);
    v->x2 = fromDouble(2);
    v->x3 = negate(power(fromDouble(10), add(b, fromDouble(1))));
    v->x4 = power(fromDouble(10), b);
    v->x5 = fromDouble(2111);
    v->x6 = power(fromDouble(10), add(b, fromDouble(3)));
}

posit dot(struct vector *v1, struct vector *v2)
{
    posit arr[6] = {multiply(v1->x1, v2->x1), multiply(v1->x2, v2->x2), multiply(v1->x3, v2->x3),
                    multiply(v1->x4, v2->x4), multiply(v1->x5, v2->x5), multiply(v1->x6, v2->x6)};
    posit a = add(arr[0], arr[2]);
    posit b = add(arr[3], arr[5]);
    posit c = add(arr[1], arr[4]);
    return add(add(a, b), c);
}

void print_vec(struct vector *v)
{
    printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", toDouble(v->x1), toDouble(v->x2), toDouble(v->x3),
           toDouble(v->x4), toDouble(v->x5), toDouble(v->x6));
}

START_TEST(test_a5)
    {
        printf("test_posit_a5\n");
        init_v1(x, fromDouble(5));
        for (int i = 0; i < 5; ++i) {
            init_v2(y, bs[i]);
            printf("%lf\n", toDouble(dot(x, y)));
            ck_assert(is_less(p_abs(sub(res, dot(x,y))), eps));
        }
    }
END_TEST

START_TEST(test_a10)
    {
        printf("test_posit_a10\n");
        init_v1(x, fromDouble(10));
        for (int i = 0; i < 5; ++i) {
            init_v2(y, bs[i]);
            printf("%lf\n", toDouble(dot(x, y)));
            ck_assert(is_less(p_abs(sub(res, dot(x,y))), eps));
        }
    }
END_TEST

void setup(void)
{
    x = calloc(1, sizeof(struct vector));
    y = calloc(1, sizeof(struct vector));

    double bs_double[5] = {5, 8, 12, 15, 20};
    for(int i = 0; i < 5; ++i)
        bs[i] = fromDouble(bs_double[i]);

    res = fromDouble(8779);
    eps = fromDouble(10e-8);
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
