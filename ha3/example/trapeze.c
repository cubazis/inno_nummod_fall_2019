//
// Created by Алексей Евлампьев on 17.10.2019.
//
#include <stdio.h>
#include "blackbox.h"

double a = -1.0;
double b = 1.0;
double eps = 1e-9;

//calculate integral with trapeze formula
int main() {
    int n;
    scanf("%d", &n);
    blackbox_init(n);
    double m2 = blackbox_df(2);

    // substituted b-a = 2 to the condition from book and get this formula
    int steps = (int)sqrt(( 2*m2) / (3 * eps)) + 1;

    double h = (b - a) / steps;
    double f0 = blackbox(a);
    double fn = blackbox(b);
    double sum = f0 + fn;
    for (int i = 1; i < steps; i++) {
        sum += 2 * blackbox(a + i * h);
    }
    sum = sum * h / 2;
    printf("%.12lf\n", sum);
}

