#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <cmath>
#include "../src/posit.h"

struct vector {
    Posit x1;
    Posit x2;
    Posit x3;
    Posit x4;
    Posit x5;
    Posit x6;
};

double bs[5] = {1, 2, 3, 4, 5};
double res = 8799;
double eps = 10e-8;

int bits = 32;
int es = 2;

vector init_v1(Posit a)
{
    vector v{
        Posit(bits, es),
        Posit(bits, es),
        Posit(bits, es),
        Posit(bits, es),
        Posit(bits, es),
        Posit(bits, es)
    };
    v.x1.set(double(pow(10, a.getDouble())));
    v.x2.set(double(1233));
    v.x3.set(double(pow(10, a.getDouble()-1)));
    v.x4.set(double(pow(10, a.getDouble()-2)));
    v.x5.set(double(3));
    v.x6.set(double(-pow(10, a.getDouble()-5)));
    return v;
}

vector init_v2(Posit b)
{
    vector v{
            Posit(bits, es),
            Posit(bits, es),
            Posit(bits, es),
            Posit(bits, es),
            Posit(bits, es),
            Posit(bits, es)
    };
    v.x1.set(double(pow(10, b.getDouble())));
    v.x2.set(double(2));
    v.x3.set(double(-pow(10, b.getDouble()+1)));
    v.x4.set(double(pow(10, b.getDouble())));
    v.x5.set(double(2111));
    v.x6.set(double(pow(10, b.getDouble()+3)));
    return v;
}

Posit dot(struct vector *v1, struct vector *v2)
{
    return v1->x1*v2->x1+v1->x2*v2->x2+v1->x3*v2->x3+v1->x4*v2->x4+v1->x5*v2->x5+v1->x6*v2->x6;
}

void print_vec(struct vector *v)
{
    printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", v->x1.getDouble(), v->x2.getDouble(),
            v->x3.getDouble(), v->x4.getDouble(), v->x5.getDouble(), v->x6.getDouble());
}

BOOST_AUTO_TEST_CASE(test_dot_product) {
    Posit a = Posit(bits, es);
    a.set(double(2));
    double a_d = a.getDouble();
    auto v1 = init_v1(a);
    for (int i = 0; i < 5; ++i) {
        Posit b = Posit(bits, es);
        b.set(bs[i]);
        auto v2 = init_v2(b);
        print_vec(&v1);
        print_vec(&v2);
        double output = dot(&v1, &v2).getDouble();
        printf("%lf\n", output);
        BOOST_CHECK(abs(res - output) < eps);
    }
}