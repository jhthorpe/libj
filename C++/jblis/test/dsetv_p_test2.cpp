#include "jblis_test.hpp"

/*
  tests if dsetv_p works with until stride = 2
*/

int dsetv_p_test2()
{
  const double A=5;
  bool fail = false;

  const long INCX = 2;
  const long N = 100;
  vec<double> X(N);
  X = 1;

  libj_dsetv_p(N,A,&X[0],INCX);

  for (int i=0;i<X.size();i+=INCX)
  {
    if (X(i) != A) {fail = true;break;} 
  }

  if (fail) 
  {
    printf("dsetv_p_test2 failed \n");
    return 1;
  } else {
    printf("dsetv_p_test2 passed\n"); 
    return 0;
  }
  X.free();
}

