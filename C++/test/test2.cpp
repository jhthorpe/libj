#include "libj.hpp"
#include "def.hpp"
#include <stdio.h>

int main()
{
  const long N = 5;
  vec<double>W;
  vec<double>X;
  vec<double>Y;
  vec<double>Z;
  W.aligned_allocate(DOUBLE_ALIGN,N);
  X.aligned_allocate(DOUBLE_ALIGN,N);
  Y.aligned_allocate(DOUBLE_ALIGN,N);
  Z.aligned_allocate(DOUBLE_ALIGN,N);

  double dp;

  W = 1;
  X = 2;
  Y = 3;

  printf("\n W \n");
  W.print();
  printf("\n X \n");
  X.print();
  printf("\n Y \n");
  Y.print();
  simd_wxy_mul<double,DOUBLE_ALIGN>(N,&W[0],&X[0],&Y[0],&Z[0]);
  printf("\n Z \n");
  Z.print();

  printf("\n reduction is %lf \n",simd_reduction_add<double,DOUBLE_ALIGN>(N,&Z[0]));  

  printf("\n other reduction is %lf \n",simd_dotwxy<double,DOUBLE_ALIGN>(N,&W[0],&X[0],&Y[0]));

  return 0;
}
