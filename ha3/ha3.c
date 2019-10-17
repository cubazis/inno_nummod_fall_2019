#include <stdio.h>
#include "blackbox.h"

double a = -1.0, b = 1.0;
int i;

double f(int n) {
    double h = (b - a) / n;
    double sum = 0;
    for (i = 1; i < n; i += 2) {
        sum += blackbox(a + (i - 1) * h) + 4 * blackbox(a + i * h) + blackbox(a + (i + 1) * h);
    }
    return h * sum / 3;
}

int main() {
    scanf("%d", &i);
    blackbox_init(i);

    double res = f((int)2e4);
    printf("%.10f", res);

    return 0;
}

