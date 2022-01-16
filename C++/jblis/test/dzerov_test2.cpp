#include "jblis_test.hpp"

//tests unit stride
int dzerov_test2()
{
  const double A=0;
  bool fail = false;

  const long N = 100;
  const long INCX = 2;
  vec<double> X(N);
  X = 1;

  libj_dzerov(N,&X[0],INCX);

  for (int i=0;i<N;i+=INCX)
  {
    if (X(i) != A) {fail = true;break;} 
  }

  if (fail) 
  {
    printf("dzerov_test2 failed \n");
    return 1;
  } else {
    printf("dzerov_test2 passed\n"); 
    return 0;
  }
  X.free();
}

