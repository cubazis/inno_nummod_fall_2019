#include <stdio.h>
#include "blackbox.h"


int main() {
	int n;
	scanf("%d", &n);
	blackbox_init(n);

// this num_steps is enough to pass tests
	const size_t num_steps = 18600;
//    double h = 2.0000 / num_steps;
    double h = 1.0 / 9300;


// simpson formula
    double sum = blackbox(-1) + blackbox(-1 + num_steps * h);
	for (size_t  i = 1; i < num_steps; i++) {
	    if (i % 2 == 0){
            sum += 2 * blackbox(-1 + i * h);
	    } else{
	        sum += 4 * blackbox(-1 + i * h);
	    }
	}
	sum = sum * h / 3;

	printf("%.10f", sum);
}