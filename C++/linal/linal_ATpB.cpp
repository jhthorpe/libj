/*-------------------------------------------------
  linal_ATpB.cpp
	JHT, January 7, 2022 : created

  .cpp file for ATpB, which transposes an NxM
  matrix A, scales it, and adds it to a scaled
  MxN matrix B 

  It is assumed that A and B are stored sequentially
  in memory

Parameters
-------------------
M	const long	rows of B, cols of A
N	const long	cols of B, rows of A
ALPHA	const T		constant to multiply A^T
A*	const T*	pointer to A (NxM)
BETA	const T		constant to multiply B
B*	T*		pointer to B matrix (MxN)		

-------------------------------------------------*/
#include "linal_ATpB.hpp"

template<typename T>
void linal_ATpB(const long M, const long N, const T ALPHA, 
                const T* A, const T BETA, T* B)
{
  
  const T* AP = A; 
  T* BP = B;

  //common case, alpha == 1 beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    for (long j=0;j<N;j++)
    {
      BP = B + j*M;
      AP = A + j;
      for (long i=0;i<M;i++)
      {
        *(BP + i) = *(AP + i*N);
      } 
    }

  //common case, alpha == 1 beta == 1
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL )
  {
    for (long j=0;j<N;j++)
    {
      BP = B + j*M;
      AP = A + j;
      for (long i=0;i<M;i++)
      {
        *(BP + i) += *(AP + i*N);
      } 
    }

  //General case
  } else {
    for (long j=0;j<N;j++)
    {
      BP = B + j*M;
      AP = A + j;
      for (long i=0;i<M;i++)
      {
        *(BP + i) = ALPHA**(AP + i*N) + BETA**(BP+i);
      } 
    }
  }

}

template void linal_ATpB<double>(const long M, const long N, const double ALPHA, const double* A, const double BETA, double* B);
template void linal_ATpB<float>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);
template void linal_ATpB<long>(const long M, const long N, const long ALPHA, const long* A, const long BETA, long* B);
template void linal_ATpB<int>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);

/*
template<typename T, const int ALIGN>
void linal_ATpB(const long M, const long N, const T ALPHA, 
                const T* A, const T BETA, T* B)
{
  
  const T* AP = A; 
  T* BP = B;

  //common case, alpha == 1 beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    for (long j=0;j<N;j++)
    {
      BP = B + j*M;
      AP = A + j;
      for (long i=0;i<M;i++)
      {
        *(BP + i) = *(AP + i*N);
      } 
    }

  //common case, alpha == 1 beta == 1
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL )
  {
    for (long j=0;j<N;j++)
    {
      BP = B + j*M;
      AP = A + j;
      for (long i=0;i<M;i++)
      {
        *(BP + i) += *(AP + i*N);
      } 
    }

  //General case
  } else {
    for (long j=0;j<N;j++)
    {
      BP = B + j*M;
      AP = A + j;
      for (long i=0;i<M;i++)
      {
        *(BP + i) = ALPHA**(AP + i*N) + BETA**(BP+i);
      } 
    }
  }

}
template void linal_ATpB<double,128>(const long M, const long N, const double ALPHA, const double* A, const double BETA, double* B);
template void linal_ATpB<double,64>(const long M, const long N, const double ALPHA, const double* A, const double BETA, double* B);
template void linal_ATpB<double,32>(const long M, const long N, const double ALPHA, const double* A, const double BETA, double* B);
template void linal_ATpB<double,16>(const long M, const long N, const double ALPHA, const double* A, const double BETA, double* B);
template void linal_ATpB<double,8>(const long M, const long N, const double ALPHA, const double* A, const double BETA, double* B);

template void linal_ATpB<float,128>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);
template void linal_ATpB<float,64>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);
template void linal_ATpB<float,32>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);
template void linal_ATpB<float,16>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);
template void linal_ATpB<float,8>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);
template void linal_ATpB<float,4>(const long M, const long N, const float ALPHA, const float* A, const float BETA, float* B);

template void linal_ATpB<long,128>(const long M, const long N, const long ALPHA, const long* A, const long BETA, long* B);
template void linal_ATpB<long,64>(const long M, const long N, const long ALPHA, const long* A, const long BETA, long* B);
template void linal_ATpB<long,32>(const long M, const long N, const long ALPHA, const long* A, const long BETA, long* B);
template void linal_ATpB<long,16>(const long M, const long N, const long ALPHA, const long* A, const long BETA, long* B);
template void linal_ATpB<long,8>(const long M, const long N, const long ALPHA, const long* A, const long BETA, long* B);

template void linal_ATpB<int,128>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);
template void linal_ATpB<int,64>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);
template void linal_ATpB<int,32>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);
template void linal_ATpB<int,16>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);
template void linal_ATpB<int,8>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);
template void linal_ATpB<int,4>(const long M, const long N, const int ALPHA, const int* A, const int BETA, int* B);
*/
