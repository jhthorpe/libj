/*-----------------------------------------------------------------
  linal_vxM_small.cpp
	JHT, August 18, 2021 : created

  - multiplies, element wise, a continuous vector with a 
    continuous matrix, and stores the result in a new matrix 
-----------------------------------------------------------------*/
/*
   M    int    number of elements of x, rows of A,B
   N    int    number of cols of A,B
   x*   T      vector in
   A*   T      matrix in
   B*   T      matrix result

*/
#include "linal_vxM_small.hpp"

template <typename T>
void linal_vxM_small(const int M,const int N, T* x, T* A, T* B)
{
  const int l1=N-1;
  const int l2=M-1;
  int off,i;
  int col = 0;
  while (true)
  {
    off = col*M;
    i = 0;
    while (true)
    {
      *(B+off) = *(x+i) * *(A+off);
      if (i >= l2) break;
      off++;
      i++;
    } 
    if (col >= l1) break;
    col++;
  }
}

template void linal_vxM_small<double>(const int M, const int N,double* x,double* A, double* B); 
template void linal_vxM_small<float>(const int M, const int N,float* x,float* A, float* B); 
template void linal_vxM_small<int>(const int M, const int N, int* x, int* A, int* B); 
template void linal_vxM_small<long>(const int M, const int N,long* x,long* A, long* B); 
