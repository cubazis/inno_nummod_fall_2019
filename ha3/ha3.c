#include <stdio.h>
#include "blackbox.h"

double a = -1.0, b = 1.0;
int i, n;
double sum, h;

int main() {
    scanf("%d", &i);
    blackbox_init(i);

    n = (int)(2e4 - 1e3);
    h = (b - a) / n;
    sum = blackbox(a) + blackbox(b);
    i = 3;

    while (i < n) {
        sum += 2 * blackbox(a + (i - 1) * h) + 4 * blackbox(a + i * h);
        i += 2;
    }
    sum += 4 * blackbox(a + h);
    printf("%.10f", h * sum / 3);

    return 0;
}

