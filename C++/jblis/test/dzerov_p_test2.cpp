#include "jblis_test.hpp"

//tests unit stride
int dzerov_p_test2()
{
  #if defined (OMP)
  const long INCX = 2;
  const long N = 100;
  vec<double> X(INCX*N);
  X = 1;
  libj_dzerov_p(N,&X[0],2);
  double sum=0;
  for (int i=0;i<N;i+=INCX)
  {
    sum += X(i);
  }
  if (sum != 0) 
  {
    printf("dzerov_p_test2 failed : sum == %lf, not 0 \n",sum);
    return 1;
  } else {
    printf("dzerov_p_test2 passed\n"); 
    return 0;
  }
  X.free();
  #else
    return 0;
  #endif
}

