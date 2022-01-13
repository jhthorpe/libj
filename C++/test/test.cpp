#include <stdio.h>
#include "libj.hpp"

int main()
{
  const long M = 5;
  vec<double> V1;
  vec<double> V2;
  vec<double> V3;
  
  V1.aligned_allocate(64,M);
  V2.aligned_allocate(64,M);
  V3.aligned_allocate(64,M);

  V1(0) = 1;
  V1(1) = 2;
  V1(2) = 3;
  V1(3) = 4;
  V1(4) = 5;
  
  V2(0) = 2;
  V2(1) = 3;
  V2(2) = 4;
  V2(3) = 5;
  V2(4) = 6;


  printf("\nv1\n");
  V1.print();
  printf("\nv2\n");
  V2.print();
  simd_axpby<double>(M,0.1,&V1[0],1,&V2[0]);
  printf("\nnewv2\n");
  V2.print();
  

}
