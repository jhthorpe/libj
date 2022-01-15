#include "jblis_test.hpp"

//tests unit stride
int dzerov_test1()
{
  const long N = 100;
  vec<double> X(N);
  X = 1;
  libj_dzerov(N,&X[0],1);
  double sum=0;
  for (int i=0;i<N;i++)
  {
    sum += X(i);
  }
  if (sum != 0) 
  {
    printf("dzerov_test1 failed : sum == %lf, not 0 \n",sum);
    return 1;
  } else {
    printf("dzerov_test1 passed\n"); 
    return 0;
  }
  X.free();
}

