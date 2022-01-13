/*-------------------------------------------------------------
  linal_diag_ABpC.cpp
	JHT, December 30, 2021 : created

  .cpp file for linal_diag_ABpC, which calcules only the 
  M diagonal elements resulting from the following matrix
  operation:

  diag C = diag ( alpha*A*B + beta*C )

  NOTE : it is assumed all matrices and vectors are 
         stored sequentially in memory 

Parameters
M	long		rows
N	long		cols
ALPHA	const T		constant to multiply A	
A	const double*	A (MxN)
B	const double*   B (NxM)
BETA	const T		constant to multiply C
C	T*		C (M) diagonal elements
-------------------------------------------------------------*/
#include "linal_diag_ABpC.hpp"

template<typename T>
void linal_diag_ABpC(const long M, const long N, const T ALPHA,
                     const T* A, const T* B, const T BETA, T* C)
{
  //common case of alpha = 1 and beta = 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA ) < DZTOL) 
  {
    for (long j=0;j<M;j++)
    {
      *(C+j) = 0;
      for (long k=0;k<N;k++)
      {
        *(C+j) += *(A+M*k+j) * *(B+N*j+k);
      }
    }  
  } else {
    for (long j=0;j<M;j++)
    {
      *(C+j) *= BETA;
      for (long k=0;k<N;k++)
      {
        *(C+j) += ALPHA *  *(A+M*k+j) * *(B+N*j+k);
      }
    }
  }
}
template void linal_diag_ABpC<double>(const long M, const long N, const double ALPHA, const double* A, const double* B, const double BETA, double* C);
template void linal_diag_ABpC<float>(const long M, const long N, const float ALPHA, const float* A, const float* B, const float BETA, float* C);
template void linal_diag_ABpC<long>(const long M, const long N, const long ALPHA, const long* A, const long* B, const long BETA, long* C);
template void linal_diag_ABpC<int>(const long M, const long N, const int ALPHA, const int* A, const int* B, const int BETA, int* C);

