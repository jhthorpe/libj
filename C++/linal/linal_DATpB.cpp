/*-----------------------------------------------------------
  linal_DATpB.cpp
	JHT, December 30, 2021 : created

  .hpp file for linal_DATpB, which multiplies a scaled diagonal
  matrix (D) of M elements against the transpose of a matrix
  A of dimension (NxM) and adds the result to the scaled
  matrix B (MxN) 

  B = alpha*D*A^T + beta*B

Parameters
M       long            rows of D,B : cols of A
N       long            cols of D   : rows of A
ALPHA   const T         constant to multiple D
D       const T*        vector of diagonal matrix (M)
A       const T*        matrix to multiply (NxM)
BETA    const T         constant to multiply B
B       T*              resulting matrix (MxN)

This routine could probably use with a blocking algorithm


-----------------------------------------------------------*/
#include "linal_DATpB.hpp"

#include <stdio.h>

template<typename T>
void linal_DATpB(const long M, const long N, const T ALPHA, const T* D, 
                const T* A, const T BETA, T* B)
{
  const T* AP;
  T* BP;
  //Common case of ALPHA=1,BETA=0
  if (fabs((double)ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA) < DZTOL)
  {
    for (long j=0;j<N;j++)
    {
      AP = A+j;
      BP = B+M*j;
      for (long i=0;i<M;i++)
      {
        *(BP+i) = *(D+i)**(AP+N*i);
      }
    }

  //General case
  } else {
    for (long j=0;j<N;j++)
    {
      AP = A+j;
      BP = B+M*j;
      for (long i=0;i<M;i++)
      {
        *(BP+i) = ALPHA**(D+i)**(AP+N*i) + BETA**(BP+i);
      }
    }

  }
}
template void linal_DATpB<double>(const long M, const long N, const double ALPHA, const double* D, const double* A, const double BETA, double* B);
template void linal_DATpB<float>(const long M, const long N, const float ALPHA, const float* D, const float* A, const float BETA, float* B);
template void linal_DATpB<long>(const long M, const long N, const long ALPHA, const long* D, const long* A, const long BETA, long* B);
template void linal_DATpB<int>(const long M, const long N, const int ALPHA, const int* D, const int* A, const int BETA, int* B);

