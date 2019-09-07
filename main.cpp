#include "posit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std;

const int number_of_tests = 5;
int bs[5] = {5, 8, 12, 15, 20}; 
double eps = 10e-8;

bool test(int aa, int bb){
  float a = (float)aa;
  float b = (float)bb;
  Posit32 res(8779.f); //result that we want to get

  Posit32 x11(pow(10, a)); //first vector initialization
  Posit32 x12(1223.f);
  Posit32 x13(pow(10, a-1));
  Posit32 x14(pow(10, a-2));
  Posit32 x15(3.f);
  Posit32 x16(-pow(10, a-5));

  Posit32 x21(pow(10, b)); //second vector initialization
  Posit32 x22(2.f);
  Posit32 x23(-pow(10, b+1));
  Posit32 x24(pow(10, b));
  Posit32 x25(2111.f);
  Posit32 x26(pow(10, b+3));
  
  Posit32 dot(x11*x21 + x13*x23 + x14*x24 + x16*x26 + x12*x22 + x15*x25); //dot-product

  Posit32 diff(dot - res); //check for correctness
  if (abs(diff.getDouble()) > eps) 
    return false;
  else 
    return true;
}

int main (int argc, char *argv[]) {
  int correct = 0;
  for (int i = 0; i < number_of_tests; i++) {
    if (test(5, bs[i])) 
      correct++;
  }
  cout << correct << " correct tests passed out of " << number_of_tests;
}