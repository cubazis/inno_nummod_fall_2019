#include "pch.h"
#include "CppUnitTest.h"
#include "posit.h"
#include "posit.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testdotproduct
{
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

	posit bs[5] = { convertDoubleToP32(5), convertDoubleToP32(8), convertDoubleToP32(12), convertDoubleToP32(15), convertDoubleToP32(20) };
	posit res = convertDoubleToP32(8779);
	posit eps = convertDoubleToP32(10e-8);

	void init_v1(struct vector* v, posit a)
	{
		v->x1 = power(convertDoubleToP32(10), a);;
		v->x2 = convertDoubleToP32(1223);
		v->x3 = power(convertDoubleToP32(10), p32_sub(a, convertDoubleToP32(1)));
		v->x4 = power(convertDoubleToP32(10), p32_sub(a, convertDoubleToP32(2)));
		v->x5 = convertDoubleToP32(3);
		v->x6 = p32_mul(convertDoubleToP32(-1), power(convertDoubleToP32(10), p32_sub(a, convertDoubleToP32(5))));
	}

	void init_v2(struct vector* v, posit b)
	{
		v->x1 = power(convertDoubleToP32(10), b);
		v->x2 = convertDoubleToP32(2);
		v->x3 = p32_mul(convertDoubleToP32(-1), power(convertDoubleToP32(10),p32_add(b, convertDoubleToP32(1))));
		v->x4 = power(convertDoubleToP32(10), b);
		v->x5 = convertDoubleToP32(2111);
		v->x6 = power(convertDoubleToP32(10), p32_add(b, convertDoubleToP32(3)));
	}

	posit dot(struct vector* v1, struct vector* v2)
	{
		posit arr[6] = { p32_mul(v1->x1, v2->x1), p32_mul(v1->x2, v2->x2), p32_mul(v1->x3, v2->x3),
					p32_mul(v1->x4, v2->x4), p32_mul(v1->x5, v2->x5), p32_mul(v1->x6, v2->x6) };
		posit a = p32_add(arr[0], arr[2]);
		posit b = p32_add(arr[3], arr[5]);
		posit c = p32_add(arr[1], arr[4]);
		return p32_add(p32_add(a, b), c);
	}

	void print_vec(struct vector* v)
	{
		printf("{%lf, %lf, %lf, %lf, %lf, %lf}\n", convertP32ToDouble(v->x1), convertP32ToDouble(v->x2), 
			convertP32ToDouble(v->x3), convertP32ToDouble(v->x4), convertP32ToDouble(v->x5), convertP32ToDouble(v->x6));
	}

	TEST_CLASS(Testdotproduct)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			struct vector x, y;

			printf("test_double_a5\n");
			init_v1(&x, convertDoubleToP32(5));
			for (int i = 0; i < 5; ++i) {
				init_v2(&y, bs[i]);
				printf("%lf\n", convertP32ToDouble(dot(&x, &y)));
				Assert::AreEqual(fabs(convertP32ToDouble(p32_sub(res, dot(&x, &y)))) < convertP32ToDouble(eps), true);
			}
		}

		TEST_METHOD(TestMethod2)
		{
			struct vector x, y;

			printf("test_double_a10\n"); 
			init_v1(&x, convertDoubleToP32(10));
			for (int i = 0; i < 5; ++i) {
				init_v2(&y, bs[i]);
				printf("%lf\n", convertP32ToDouble(dot(&x, &y)));
				Assert::AreEqual(fabs(convertP32ToDouble(p32_sub(res, dot(&x, &y)))) < convertP32ToDouble(eps), true);
			}
		}
	};
}
