#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE DotTest
#include <boost/test/unit_test.hpp>
#include <exception>
#include <unistd.h>

#include "../lib/posit.h"

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct vector {
    Posit x1;
    Posit x2;
    Posit x3;
    Posit x4;
    Posit x5;
    Posit x6;
};

const int N = 5;

struct vector* x;
struct vector* y;

double bs[N] = {1,2,3,4,5};
double expected_res = 8779;
double eps = 1e-8;

int bits = 32;
int es = 2;


// void print_vec(struct vector *v)
// {
//     printf("{%.8lf, %.8lf, %.8lf, %.8lf, %.8lf, %.8lf}\n", v->x1, v->x2, v->x3, v->x4, v->x5, v->x6);
// }


void init_vec(struct vector *v) 
{
    v->x1 = Posit(bits, es);
    v->x2 = Posit(bits, es);
    v->x3 = Posit(bits, es);
    v->x4 = Posit(bits, es);
    v->x5 = Posit(bits, es);
    v->x6 = Posit(bits, es);
}

void init_a(struct vector *v, double a)
{
    init_vec(v);
    v->x1.set(double(pow(10, a)));
    v->x2.set(double(1223));
    v->x3.set(double(pow(10, a-1)));
    v->x4.set(double(pow(10, a-2)));
    v->x5.set(double(3));
    v->x6.set(double(-pow(10, a-5)));
}

void init_b(struct vector *v, double b)
{
    init_vec(v);
    v->x1.set(double(pow(10, b)));
    v->x2.set(double(2));
    v->x3.set(double(-pow(10, b+1)));
    v->x4.set(double(pow(10, b)));
    v->x5.set(double(2111));
    v->x6.set(double(pow(10, b+3)));
}

double dotPosit(struct vector *v1, struct vector *v2)
{
    return v1->x1.mul(v2->x1).add(
        v1->x2.mul(v2->x2)).add(
        v1->x3.mul(v2->x3)).add(
        v1->x4.mul(v2->x4)).add(
        v1->x5.mul(v2->x5)).add(
        v1->x6.mul(v2->x6)).getDouble();
}


BOOST_AUTO_TEST_SUITE(PositSuite)

BOOST_AUTO_TEST_CASE(positDotTest)
{
    int number_passed = 0;

    x = (vector*)calloc(1, sizeof(struct vector));
    y = (vector*)calloc(1, sizeof(struct vector));

    init_a(x, 5);

    for(int i=0; i < N; ++i){
        init_b(y, bs[i]);
        double res = dotPosit(x,y);
        if(fabs(res - expected_res) < eps){
            ++number_passed;
        }
        printf("Check if %.8lf == %.8lf\n", res, expected_res);
    }

    free(x);
    free(y);

    printf("Passed %d of %d test cases.\n\n", number_passed, N);

    BOOST_CHECK(number_passed == N);
}

double dotMixed(struct vector *v1, struct vector *v2)
{
    return  v1->x1.mul(v2->x1).getDouble() + 
            v1->x2.mul(v2->x2).getDouble() + 
            v1->x3.mul(v2->x3).getDouble() + 
            v1->x4.mul(v2->x4).getDouble() + 
            v1->x5.mul(v2->x5).getDouble() + 
            v1->x6.mul(v2->x6).getDouble();
}

BOOST_AUTO_TEST_CASE(positDotMixedTest)
{
    int number_passed = 0;

    x = (vector*)calloc(1, sizeof(struct vector));
    y = (vector*)calloc(1, sizeof(struct vector));

    init_a(x, 5);

    for(int i=0; i < N; ++i){
        init_b(y, bs[i]);
        double res = dotMixed(x,y);
        if(fabs(res - expected_res) < eps){
            ++number_passed;
        }
        printf("Check if %.8lf == %.8lf\n", res, expected_res);
    }

    free(x);
    free(y);

    printf("Passed %d of %d test cases.\n\n", number_passed, N);

    BOOST_CHECK(number_passed == N);
}



double dotDoubles(struct vector *v1, struct vector *v2)
{
    return  v1->x1.getDouble() * (v2->x1).getDouble() + 
            v1->x2.getDouble() * (v2->x2).getDouble() + 
            v1->x3.getDouble() * (v2->x3).getDouble() + 
            v1->x4.getDouble() * (v2->x4).getDouble() + 
            v1->x5.getDouble() * (v2->x5).getDouble() + 
            v1->x6.getDouble() * (v2->x6).getDouble();
}

BOOST_AUTO_TEST_CASE(positDotDoublesTest)
{
    int number_passed = 0;

    x = (vector*)calloc(1, sizeof(struct vector));
    y = (vector*)calloc(1, sizeof(struct vector));

    init_a(x, 5);

    for(int i=0; i < N; ++i){
        init_b(y, bs[i]);
        double res = dotDoubles(x,y);
        if(fabs(res - expected_res) < eps){
            ++number_passed;
        }
        printf("Check if %.8lf == %.8lf\n", res, expected_res);
    }

    free(x);
    free(y);

    printf("Passed %d of %d test cases.\n\n", number_passed, N);

    BOOST_CHECK(number_passed == N);
}
// BOOST_AUTO_TEST_CASE(multipleCheckFailures)
// {
//     BOOST_CHECK(add(2, 2) == 1);
//     BOOST_CHECK(add(2, 2) == 2);
//     BOOST_CHECK(add(2, 2) == 3);
// }

// BOOST_AUTO_TEST_CASE(requireFailure)
// {
//     BOOST_REQUIRE(add(2, 2) == 5);
// }

// BOOST_AUTO_TEST_CASE(explicitError)
// {
//     BOOST_ERROR("Error message");
// }

// BOOST_AUTO_TEST_CASE(explicitFailure)
// {
//     BOOST_FAIL("Failure message");
// }

// BOOST_AUTO_TEST_CASE(errorThenFailure)
// {
//     BOOST_FAIL("Error message");
//     BOOST_FAIL("Failure message");
// }

// BOOST_AUTO_TEST_CASE(uncaughtException)
// {
//     throw "Catch me if you can!";
// }

// BOOST_AUTO_TEST_CASE(stdException)
// {
//     throw new std::exception();
// }

// BOOST_AUTO_TEST_CASE(checkMessageFailure)
// {
//     BOOST_CHECK_MESSAGE(add(2, 2) == 5, "add(..) result: " << add(2, 2));
// }

// BOOST_AUTO_TEST_CASE(checkEqualFailure)
// {
//     BOOST_CHECK_EQUAL(add( 2,2 ), 5);
// }


BOOST_AUTO_TEST_SUITE_END()

