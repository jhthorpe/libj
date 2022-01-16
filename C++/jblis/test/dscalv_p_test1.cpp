#include "jblis_test.hpp"

//tests unit stride
int dscalv_p_test1()
{
  const double A=5;
  bool fail = false;

  const long INCX = 1;
  const long N = 100;
  vec<double> X(N);
  X = 1;

  libj_dscalv_p(N,A,&X[0],INCX);

  for (long i=0;i<N;i+=INCX)
  {
    if (X(i) != A) 
    {
      printf("index %ld value is %lf, should be %lf\n",i,X(i),A);
      fail = true;
      break;
    } 
  }

  if (fail) 
  {
    printf("dscalv_p_test1 failed \n");
    return 1;
  } else {
    printf("dscalv_p_test1 passed\n"); 
    return 0;
  }
  X.free();
}

