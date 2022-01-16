#include "jblis_test.hpp"


/*
  Tests if dscalv works, with unit stride of 2
*/
int dscalv_test2()
{
  const double A=5;
  bool fail = false;

  const long INCX = 2;
  const long N = 100;
  vec<double> X(N);

  X = 1;
  libj_dscalv(N,A,&X[0],INCX);

  for (int i=0;i<X.size();i+=INCX)
  {
    if (X(i) != A) {fail = true;break;} 
  }

  if (fail) 
  {
    printf("dscalv_test2 failed \n");
    return 1;
  } else {
    printf("dscalv_test2 passed\n"); 
    return 0;
  }
  X.free();
}

