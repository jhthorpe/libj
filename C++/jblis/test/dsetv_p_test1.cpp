#include "jblis_test.hpp"

/*
  tests if dsetv_p works with until stride = 1
*/

int dsetv_p_test1()
{
  const double A=5;
  bool fail = false;

  const long INCX = 1;
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
    printf("dsetv_p_test1 failed \n");
    return 1;
  } else {
    printf("dsetv_p_test1 passed\n"); 
    return 0;
  }
  X.free();
}

