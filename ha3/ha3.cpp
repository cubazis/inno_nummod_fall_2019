#include <cstdio>
#include "blackbox.h"

double a = -1.0, b = 1.0;

inline double f(int n) {
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 1; i < n; i += 2) {
        sum += blackbox(a + (i - 1) * h) + 4 * blackbox(a + i * h) + blackbox(a + (i + 1) * h);
    }
    return h * sum / 3;
}

int main() {
    int init;
    scanf("%d", &init);
    blackbox_init(init);

    double res = f((int)1e6);
    printf("%.10f", res);

    return 0;
}
