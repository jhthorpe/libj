/*------------------------------------------------------------
  linal_ATDApU.cpp
	JHT, January 1, 2022 : created

  .cpp file for the linal_ATDApU function, which performs
  the following operation:

  U = A^T . alpha*D . A + beta*U

  where U is a upper symmetric matrix (MxM), A is a general 
  (NxM) matrix, and D is a diagonal (NxN) matrix.

  It is assumed that A,D, and U are stored sequentially 
  in memory, column major, and that only the diagonal (N) 
  elements of D, and the upper triangular (M*(M+1)/2) elements 
  of U are stored

Parameters
M       const long      rows of U,A^T | cols of D,A
N       const long      rows of A     | cols of A^T     
A       const T*        A matrix (NxM)
ALPHA   const T         constant multipling D 
D       const T*        D diagonal elements (N)
BETA    const T         beta constant to multiply U
U       T*              U upper triangular elements (M*(M+1)/2) 


------------------------------------------------------------*/
#include "linal_ATDApU.hpp"

template<typename T>
void linal_ATDApU(const long M, const long N, const T* A, const T ALPHA, const T* D, const T BETA, T* U)
{
  const T* AP;
  T* UP;
  //common case: ALPHA == 1 BETA == 0
  if (fabs((double) ALPHA - 1) < DZTOL && 
      fabs((double) BETA     ) < DZTOL )
  {
    for (long j=0;j<M;j++)
    {
      AP = A+N*j;
      UP = U+(j*(j+1))/2;
      for (long i=0;i<=j;i++)
      {
        *(UP+i) = simd_dotwxy<T>(N,A+N*i,D,AP); 
      }
    }

  //common case : ALPHA == 1 BETA == 1
  } else if (fabs((double) ALPHA - 1) < DZTOL &&
             fabs((double) BETA - 1 ) < DZTOL ) {

    for (long j=0;j<M;j++)
    {
      AP = A+N*j;
      UP = U+(j*(j+1))/2;
      for (long i=0;i<=j;i++)
      {
        *(UP+i) += simd_dotwxy<T>(N,A+N*i,D,AP); 
      }
    }


  //general case
  } else {
    for (long j=0;j<M;j++)
    {
      AP = A+N*j;
      UP = U+(j*(j+1))/2;
      for (long i=0;i<=j;i++)
      {
        *(UP+i) = simd_dotwxy<T>(N,A+N*i,D,AP) + BETA**(UP+i); 
      }
    }
  }

}
template void linal_ATDApU<double>(const long M, const long N, const double* A, const double ALPHA, const double* D, const double BETA, double* U);
template void linal_ATDApU<float>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);
template void linal_ATDApU<long>(const long M, const long N, const long* A, const long ALPHA, const long* D, const long BETA, long* U);
template void linal_ATDApU<int>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);

/*
//----------------------------------------------------------------
// Aligned code
template<typename T, const int ALIGN>
void linal_ATDApU(const long M, const long N, const T* A, const T ALPHA, const T* D, const T BETA, T* U)
{
  const T* AP;
  T* UP;
  //common case: ALPHA == 1 BETA == 0
  if (fabs((double) ALPHA - 1) < DZTOL && 
      fabs((double) BETA     ) < DZTOL )
  {
    for (long j=0;j<M;j++)
    {
      AP = A+N*j;
      UP = U+(j*(j+1))/2;
      for (long i=0;i<=j;i++)
      {
        *(UP+i) = simd_dotwxy<T,ALIGN>(N,A+N*i,D,AP); 
      }
    }

  //common case : ALPHA == 1 BETA == 1
  } else if (fabs((double) ALPHA - 1) < DZTOL &&
             fabs((double) BETA - 1 ) < DZTOL ) {

    for (long j=0;j<M;j++)
    {
      AP = A+N*j;
      UP = U+(j*(j+1))/2;
      for (long i=0;i<=j;i++)
      {
        *(UP+i) += simd_dotwxy<T,ALIGN>(N,A+N*i,D,AP); 
      }
    }


  //general case
  } else {
    for (long j=0;j<M;j++)
    {
      AP = A+N*j;
      UP = U+(j*(j+1))/2;
      for (long i=0;i<=j;i++)
      {
        *(UP+i) = simd_dotwxy<T,ALIGN>(N,A+N*i,D,AP) + BETA**(UP+i); 
      }
    }
  }

}
template void linal_ATDApU<double,128>(const long M, const long N, const double* A, const double ALPHA, const double* D, const double BETA, double* U);
template void linal_ATDApU<double,64>(const long M, const long N, const double* A, const double ALPHA, const double* D, const double BETA, double* U);
template void linal_ATDApU<double,32>(const long M, const long N, const double* A, const double ALPHA, const double* D, const double BETA, double* U);
template void linal_ATDApU<double,16>(const long M, const long N, const double* A, const double ALPHA, const double* D, const double BETA, double* U);
template void linal_ATDApU<double,8>(const long M, const long N, const double* A, const double ALPHA, const double* D, const double BETA, double* U);

template void linal_ATDApU<float,128>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);
template void linal_ATDApU<float,64>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);
template void linal_ATDApU<float,32>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);
template void linal_ATDApU<float,16>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);
template void linal_ATDApU<float,8>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);
template void linal_ATDApU<float,4>(const long M, const long N, const float* A, const float ALPHA, const float* D, const float BETA, float* U);

template void linal_ATDApU<long,128>(const long M, const long N, const long* A, const long ALPHA, const long* D, const long BETA, long* U);
template void linal_ATDApU<long,64>(const long M, const long N, const long* A, const long ALPHA, const long* D, const long BETA, long* U);
template void linal_ATDApU<long,32>(const long M, const long N, const long* A, const long ALPHA, const long* D, const long BETA, long* U);
template void linal_ATDApU<long,16>(const long M, const long N, const long* A, const long ALPHA, const long* D, const long BETA, long* U);
template void linal_ATDApU<long,8>(const long M, const long N, const long* A, const long ALPHA, const long* D, const long BETA, long* U);

template void linal_ATDApU<int,128>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);
template void linal_ATDApU<int,64>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);
template void linal_ATDApU<int,32>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);
template void linal_ATDApU<int,16>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);
template void linal_ATDApU<int,8>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);
template void linal_ATDApU<int,4>(const long M, const long N, const int* A, const int ALPHA, const int* D, const int BETA, int* U);
*/
