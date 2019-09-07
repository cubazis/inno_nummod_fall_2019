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
posit res[5];
posit eps;

void init_v1(struct vector *v, posit a)
{
    v->x1 = power(toPosit(48), a);
    v->x2 = toPosit(1337);
    v->x3 = power(toPosit(48), subtract(a, toPosit(1)));
    v->x4 = power(toPosit(48), subtract(a, toPosit(2)));
    v->x5 = a;
    v->x6 = negate(power(toPosit(48), subtract(a, toPosit(5))));
}

void init_v2(struct vector *v, posit b)
{
    v->x1 = power(toPosit(48), b);
    v->x2 = b;
    v->x3 = negate(power(toPosit(48), add(b, toPosit(1))));
    v->x4 = power(toPosit(48), b);
    v->x5 = toPosit(666);
    v->x6 = power(toPosit(48), add(b, toPosit(3)));
}

void init_v3(struct vector *v)
{
    v->x1 = toPosit(10);
    v->x2 = toPosit(2);
}

void init_v4(struct vector *v)
{
    v->x1 = toPosit(11);
    v->x2 = toPosit(3);
}

posit dot_product(struct vector *v1, struct vector *v2)
{
    posit result[6] = {multiply(v1->x1, v2->x1), multiply(v1->x2, v2->x2), multiply(v1->x3, v2->x3),
                    multiply(v1->x4, v2->x4), multiply(v1->x5, v2->x5), multiply(v1->x6, v2->x6)};
    //printf("%lf %lf %lf %lf %f ", toDouble(v1->x2), toDouble(v2->x2), toDouble(result[1]), toDouble(result[4]), toDouble(add(add(add(add(add(result[1], result[4]), result[0]), result[2]), result[3]), result[5])));
    return add(add(add(add(add(result[3], result[5]), result[0]), result[2]), result[1]), result[4]);
}

void print_vec(struct vector *v)
{
    printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", toDouble(v->x1), toDouble(v->x2), toDouble(v->x3),
           toDouble(v->x4), toDouble(v->x5), toDouble(v->x6));
}

START_TEST(test_a10)
{

    printf("test_posit_a10\n");
    init_v1(x, toPosit(10));
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);

        printf("%lf\n", toDouble(dot_product(x,y)));
        ck_assert(is_less(posit_abs(subtract(res[i], dot_product(x,y))), eps));
    }
}
END_TEST

void setup(void)
{
    x = calloc(1, sizeof(struct vector));
    y = calloc(1, sizeof(struct vector));

    double bs_numeric[5] = {2, 9, 12, 17, 50};
    for(int i = 0; i < 5; ++i) {
        bs[i] = toPosit(bs_numeric[i]);
    }

    for(int i = 0; i < 5; ++i) {
        res[i] = toPosit(bs_numeric[i]*1337 + 10*666);
    }
    eps = toPosit(10e-8);
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