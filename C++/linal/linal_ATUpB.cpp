/*--------------------------------------------------
  linal_ATUpB.cpp
	JHT, December 20, 2021 : created

 .cpp file for the linal_ATUpB, which multiplies
  an MxN matrix, A, by a symmetric NxN matrix U, 
  and stores the result in an MxN matrix B. Matrices
  A and B can optionally be scaled by some constant, 
  alpha and beta, respectively:

  B = alpha*A^T*U + beta*B

  It is assumed that only the upper-triangular 
  elements of the matrix U are stored, and that
  all matrices are sequential in memory

Parameters
M	long		number of rows
N	long		number of cols
alpha	const T		scalar to multiply A
A	const T*	matrix A (MxN)
U	const T* 	matrix U ((NxN+1)/2)
beta	cont T		scalar to multiply B
B	T*	 	matrix B (MxN)
--------------------------------------------------*/
#include "linal_geprint.hpp"
#include "linal_ATUpB.hpp"
#include <stdio.h>
template<typename T>
void linal_ATUpB(const long M, const long N, const T ALPHA, const T* A,
                 const T* U, const T BETA, T* B)
{

  //if alpha is 1 and beta is 0, a common test case
  if (fabs((double)ALPHA - (double) 1) < DZTOL 
   && fabs((double) BETA) < DZTOL)
  {
    //columns of B
    for (long j=0;j<N;j++)
    {

      //rows of B
      for (long i=0;i<M;i++)
      {

        //the up to j+1 elements of U are sequential in memory
        *(B+M*j+i) = simd_dot<T>(j+1,A+N*i,U+(j*(j+1))/2 );

        //the rest are out of order
        //note that the k'th element of the j'th col of U
        // is stored with the indices reversed, as it is 
        //below the diagonal, j
        for (long k=j+1;k<N;k++)
        {
          *(B+M*j+i) += *(A+N*i+k) * *(U+(k*(k+1))/2+j) ;
        }

      }
    }

  //The general case
  } else {
    //columns of B
    for (long j=0;j<N;j++)
    {

      //rows of B
      for (long i=0;i<M;i++)
      {

        //the up to j elements of U are sequential in memory
        *(B+M*j+i) = ALPHA*simd_dot<T>(j+1,A+N*i,U+(j*(j+1))/2 ) + BETA**(B+M*j+i);

        //the rest are sequential
        //note that the k'th element of the j'th col of U
        // is stored with the indices reversed, as it is 
        //below the diagonal, j
        for (long k=j+1;k<N;k++)
        {
          *(B+M*j+i) += ALPHA**(A+N*i+k) * *(U+(k*(k+1))/2+j) + BETA**(B+M*j+i);
        }

      }
    }

  }
  
}
template void linal_ATUpB<double>(const long M, const long N, const double ALPHA, 
                                  const double* A, const double* U,
                                  const double BETA, double* B);
template void linal_ATUpB<float>(const long M, const long N, const float ALPHA,
                                 const float* A, const float* U,
                                 const float BETA, float* B);
template void linal_ATUpB<long>(const long M, const long N, const long ALPHA, 
                                const long* A, const long* U,
                                const long BETA, long* B);
template void linal_ATUpB<int>(const long M, const long N, const int ALPHA,
                               const int* A, const int* U,
                               const int BETA, int* B);

/*
template<typename T, const int ALIGN>
void linal_ATUpB(const long M, const long N, const T ALPHA, const T* A,
                 const T* U, const T BETA, T* B)
{

  //if alpha is 1 and beta is 0, a common test case
  if (fabs((double)ALPHA - (double) 1) < DZTOL 
   && fabs((double) BETA) < DZTOL)
  {
    //columns of B
    for (long j=0;j<N;j++)
    {

      //rows of B
      for (long i=0;i<M;i++)
      {

        //the up to j elements of U are sequential in memory
        *(B+M*j+i) = simd_dot<T,ALIGN>(j+1,A+N*i,U+(j*(j+1))/2 );

        //the rest are sequential
        //note that the k'th element of the j'th col of U
        // is stored with the indices reversed, as it is 
        //below the diagonal, j
        for (long k=j+1;k<N;k++)
        {
          *(B+M*j+i) += *(A+N*i+k) * *(U+(k*(k+1))/2+j) ;
        }

      }
    }

  //The general case
  } else {
    //columns of B
    for (long j=0;j<N;j++)
    {

      //rows of B
      for (long i=0;i<M;i++)
      {

        //the up to j elements of U are sequential in memory
        *(B+M*j+i) = ALPHA*simd_dot<T,ALIGN>(j+1,A+N*i,U+(j*(j+1))/2 ) + BETA**(B+M*j+i);

        //the rest are sequential
        //note that the k'th element of the j'th col of U
        // is stored with the indices reversed, as it is 
        //below the diagonal, j
        for (long k=j+1;k<N;k++)
        {
          *(B+M*j+i) += ALPHA**(A+N*i+k) * *(U+(k*(k+1))/2+j) + BETA**(B+M*j+i);
        }

      }
    }

  }
}
template void linal_ATUpB<double,64>(const long M, const long N, const double ALPHA,const double* A, const double* U, const double BETA, double* B);
template void linal_ATUpB<double,32>(const long M, const long N, const double ALPHA,const double* A, const double* U, const double BETA, double* B);
template void linal_ATUpB<double,16>(const long M, const long N, const double ALPHA,const double* A, const double* U, const double BETA, double* B);
template void linal_ATUpB<double,8>(const long M, const long N, const double ALPHA,const double* A, const double* U, const double BETA, double* B);

template void linal_ATUpB<float,64>(const long M, const long N, const float ALPHA,const float* A, const float* U,const float BETA, float* B);
template void linal_ATUpB<float,32>(const long M, const long N, const float ALPHA,const float* A, const float* U,const float BETA, float* B);
template void linal_ATUpB<float,16>(const long M, const long N, const float ALPHA,const float* A, const float* U,const float BETA, float* B);
template void linal_ATUpB<float,8>(const long M, const long N, const float ALPHA,const float* A, const float* U,const float BETA, float* B);
template void linal_ATUpB<float,4>(const long M, const long N, const float ALPHA,const float* A, const float* U,const float BETA, float* B);


template void linal_ATUpB<long,64>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long BETA, long* B);
template void linal_ATUpB<long,32>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long BETA, long* B);
template void linal_ATUpB<long,16>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long BETA, long* B);
template void linal_ATUpB<long,8>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long BETA, long* B);

template void linal_ATUpB<int,64>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int BETA, int* B);
template void linal_ATUpB<int,32>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int BETA, int* B);
template void linal_ATUpB<int,16>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int BETA, int* B);
template void linal_ATUpB<int,8>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int BETA, int* B);
template void linal_ATUpB<int,4>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int BETA, int* B);
*/
