#include <iostream>

#include "posit.h"

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    double  a = 1e-17;
    double b =1e-16;
    Posit p1 = Posit(a);
    Posit p2 =Posit(b);
    Posit sum = p1*p2;
    cout<<sum.toDouble();
    return 0;
}