/*-----------------------------------------------------------------
  linal_vxv_small.cpp
	JHT, August 18, 2021 : created

  - multiplies two small vectors together and returns the result in 
    a new vector. Assumes that the vectors are stored sequentially
-----------------------------------------------------------------*/
#include "linal_vxv_small.hpp"
template <typename T>
void linal_vxv_small(int N, T* x, T* y, T* z)
{
  const int l1=N-1;
  int i=0;
  while (true)
  {
    *(z+i) = *(x+i) * *(y+i);
    if (i >= l1) break;
    i++; 
  } 
}

template void linal_vxv_small<double>(int N,double* x, double* y, double* z); 
template void linal_vxv_small<float>(int N,float* x, float* y, float* z); 
template void linal_vxv_small<int>(int N,int* x, int* y, int* z); 
template void linal_vxv_small<long>(int N,long* x, long* y, long* z); 

