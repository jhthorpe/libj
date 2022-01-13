/*-----------------------------------------------------------------
  linal_DApB.cpp
	JHT, August 18, 2021 : created

  Multiplies a scaled diagonal matriD, D (MxM), 
  by a rectangular matrix A(MxN), and adds this to 
  a scaled matrix B (MxN). It is assumed  
  that all elements are stored sequentially in memory.

  B = alpha*D.A + beta*B 

  Parameters
  M     long            Rows of D,A,B
  N     long            Cols of A,B
  ALPHA const T         alpha constant multiplies D
  D     const T*        Diagonal elements of X(M),       
  A     const T*        A Matrix (MxN)
  BETA  const T         beta constant multiplies B      
  B     T*              B matrix (MxN)


-----------------------------------------------------------------*/
#include "linal_DApB.hpp"

template <typename T>
void linal_DApB(const long M, const long N, const T ALPHA, const T* D, const T* A, const T BETA, T* B)
{
  //common case, ALPHA == 1, BETA == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA) < DZTOL)
  {
    for (long j=0;j<N;j++)
    {
      simd_elemwise_mul<T>(M,D,A,B);
      A+=M;
      B+=M;
    }

  //D and B need to be scaled
  } else {
    for (long j=0;j<N;j++)
    {
      //this should probably be put in SIMD form...
      for (long i=0;i<M;i++)
      {
        *(B+i) = ALPHA**(D+i)**(A+i) + BETA**(B+i);
      }
      A+=M;
      B+=M;
    }
  } 

}
template void linal_DApB<double>(const long M, const long N, const double ALPHA, const double* D, const double* A, const double BETA, double* B);
template void linal_DApB<float>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);
template void linal_DApB<long>(const long M, const long N, const long ALPHA, const long* D, const long* A, const long BETA, long* B);
template void linal_DApB<int>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);

/*
template <typename T, const int ALIGN>
void linal_DApB(const long M, const long N, const T ALPHA, const T* D, const T* A, const T BETA, T* B)
{
  //common case, ALPHA == 1, BETA == 0
  if (fabs((double)ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA) < DZTOL)
  {
    for (long j=0;j<N;j++)
    {
      simd_elemwise_mul<T,ALIGN>(M,D,A,B);
      A+=M;
      B+=M;
    }

  //D and B need to be scaled
  } else {
    for (long j=0;j<N;j++)
    {
      //this should probably be put in SIMD form...
      for (long i=0;i<M;i++)
      {
        *(B+i) = ALPHA**(D+i)**(A+i) + BETA**(B+i);
      }
      A+=M;
      B+=M;
    }
  } 

}
template void linal_DApB<double,64>(const long M, const long N, const double ALPHA, const double* D, const double* A, const double BETA, double* B);
template void linal_DApB<double,32>(const long M, const long N, const double ALPHA, const double* D, const double* A, const double BETA, double* B);
template void linal_DApB<double,16>(const long M, const long N, const double ALPHA, const double* D, const double* A, const double BETA, double* B);
template void linal_DApB<double,8>(const long M, const long N, const double ALPHA, const double* D, const double* A, const double BETA, double* B);

template void linal_DApB<float,64>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);
template void linal_DApB<float,32>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);
template void linal_DApB<float,16>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);
template void linal_DApB<float,8>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);
template void linal_DApB<float,4>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);

template void linal_DApB<long,64>(const long M, const long N, const long ALPHA, const long* D, const long* A, const long BETA, long* B);
template void linal_DApB<long,32>(const long M, const long N, const long ALPHA, const long* D, const long* A, const long BETA, long* B);
template void linal_DApB<long,16>(const long M, const long N, const long ALPHA, const long* D, const long* A, const long BETA, long* B);
template void linal_DApB<long,8>(const long M, const long N, const long ALPHA, const long* D, const long* A, const long BETA, long* B);

template void linal_DApB<int,64>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);
template void linal_DApB<int,32>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);
template void linal_DApB<int,16>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);
template void linal_DApB<int,8>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);
template void linal_DApB<int,4>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);
*/
