/*-----------------------------------------------------------------
  linal_scal_small.cpp
	JHT, August 18, 2021 : created

  - multiplies a continuous vector by a scalar, without loop unrolling 
-----------------------------------------------------------------*/
#include "linal_scal_small.hpp"
template <typename T>
void linal_scal_small(int N, T* x, T a)
{
  const int l1=N-1;
  int i=0;
  while (true)
  {
    *(x+i) *= a;
    if (i >= l1) break;
    i++; 
  } 
}

template void linal_scal_small<double>(int N,double* x, double a); 
template void linal_scal_small<float>(int N,float* x, float a); 
template void linal_scal_small<int>(int N,int* x, int a); 
template void linal_scal_small<long>(int N,long* x, long a); 

