// posit<32, 4>

#include <iostream>
#include "posit.h"

using namespace std;

int main() {
    Posit x = Posit(5);
    x.print_p();
    Posit y = Posit(6);
    y.print_p();
    y = x * y;
    y.print_p();
    cout << y.to_float() << endl;
    return 0;
}