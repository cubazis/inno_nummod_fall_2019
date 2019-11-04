#include "blackbox.h"

int n;
int i, j;
double v;

static inline double dot(double *a, double *b) {
	for (i = 0, v = .0; i < n; ++i)
		v += *(a + i) * *(b + i);
	return v;
}

static inline double MSE(double* r) {
	for (i = 0, v = .0; i < n; ++i)
		v += *(r + i) * *(r + i);
	return v;
}

main()
{
	blackbox_init();
	n = blackbox_size();
	double *b = malloc(n << 3);
	double *x = malloc(n << 3);
    double *r = malloc(n << 3);
	double *Au = malloc(n << 3);
	double *Ar = malloc(n << 3);
    blackbox_rhs(b);

	blackbox_mult(x, Au);
	for (i = 0; i < n; ++i)
        *(r + i) = *(Au + i) - *(b + i);

	blackbox_mult(r, Ar);

	for (j = 0; j <= 30; ++j) {
        double t = dot(Ar, r) / dot(Ar, Ar);
        for (i = 0; i < n; ++i)
            *(r + i) *= t;
		for (i = 0; i < n; ++i)
            *(x + i) -= *(r + i);
		blackbox_mult(x, Au);
		for (i = 0; i < n; ++i)
            *(r + i) = *(Au + i) - *(b + i);
		if (MSE(r) < 1e-19)
			break;
		blackbox_mult(r, Ar);
	}

	blackbox_submit(x);
}

