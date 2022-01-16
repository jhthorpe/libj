#include "jblis_test.hpp"

//tests unit stride
int dzerov_p_test1()
{
  const double A=0;
  bool fail = false;

  const long N = 100;
  vec<double> X(N);
  X = 1;

  libj_dzerov_p(N,&X[0],1);

  for (int i=0;i<N;i++)
  {
    if (X(i) != A) {fail = true;break;} 
  }

  if (fail) 
  {
    printf("dzerov_p_test1 failed \n");
    return 1;
  } else {
    printf("dzerov_p_test1 passed\n"); 
    return 0;
  }
  X.free();
}

