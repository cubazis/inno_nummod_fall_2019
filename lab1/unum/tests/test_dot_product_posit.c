#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "posit.h"
#include <math.h>


struct vector {
    simple_quire x1;
    simple_quire x2;
    simple_quire x3;
    simple_quire x4;
    simple_quire x5;
    simple_quire x6;
};

struct vector* x;
struct vector* y;




void init_v1(struct vector *v, posit32 a)
{

    v->x1 = pow_large(posit_init_double(10), getDouble(a));
    v->x2 = pow_large(posit_init_double(1223), 1);
    v->x3 = pow_large(posit_init_double(10), getDouble(sub32(a, posit32_init(1))));
    v->x4 = pow_large(posit_init_double(10), getDouble(sub32(a, posit32_init(2))));
    v->x5 = pow_large(posit_init_double(3), 1);
    v->x6 = negate(pow_large(posit_init_double(10), getDouble(a)-5));

}



void init_v2(struct vector *v, posit32 b) {

    v->x1 = pow_large(posit_init_double(10), getDouble(b));
    v->x2 = pow_large(posit_init_double(2), 1);
    v->x3 = negate(pow_large(posit_init_double(10), getDouble(add32(b, posit32_init(1)))));
    v->x4 = pow_large(posit_init_double(10), getDouble(b));
    v->x5 = pow_large(posit_init_double(2111), 1);
    v->x6 = pow_large(posit_init_double(10), getDouble(add32(b, posit32_init(3))));

}


long double dot(struct vector *v1, struct vector *v2)
{
    long double res = q_t_d(v1->x1)* q_t_d(v2->x1);
    res += q_t_d(v1->x3)* q_t_d(v2->x3);
    res += q_t_d(v1->x4)* q_t_d(v2->x4);
    res += q_t_d(v1->x6)* q_t_d(v2->x6);
    res += q_t_d(v1->x5)* q_t_d(v2->x5);
    res += q_t_d(v1->x2)* q_t_d(v2->x2);

    return res;
}

//void print_vec(struct vector *v)
//{
//
//}

START_TEST(test_a5)
        {
                posit32 result = posit_init_double(8779);
        posit32 eps = posit_init_double(10e-8);
        posit32 bs[5] = {posit_init_double(5), posit_init_double(8), posit_init_double(12), posit_init_double(15), posit_init_double(20)};
        printf("test_double_a5\n");
        init_v1(x, posit_init_double(5));
        for (int i = 0; i < 5; ++i) {
            init_v2(y, bs[i]);
            printf("%Lf\n", dot(x, y));
            ck_assert(fabs((double)(getDouble(result) - dot(x,y))) < getDouble(eps));
        }
        }
END_TEST

START_TEST(test_a10)
{
    posit32 result = posit_init_double(8779);
    posit32 eps = posit_init_double(10e-8);
    posit32 bs[5] = {posit_init_double(5), posit_init_double(8), posit_init_double(12), posit_init_double(15), posit_init_double(20)};
    printf("test_double_a5\n");
    init_v1(x, posit_init_double(10));
    for (int i = 0; i < 5; ++i) {
        init_v2(y, bs[i]);
        printf("%Lf\n", dot(x, y));
        ck_assert(fabs((double)(getDouble(result) - dot(x,y))) < getDouble(eps));
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