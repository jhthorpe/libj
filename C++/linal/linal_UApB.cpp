/*-------------------------------------------------------------------------
  linal_UApB.cpp
	JHT, January 11, 2022 : created

  .cpp file for UApB, which multiplies a scaled, upper symmetric matrix
  U of size MxM by matrix A of size MxN, and adds the result to a 
  scaled matrix B of size MxN

  It is assumes that U,A, and B are stored continously in memory, and
  that only the upper symmetric parts of U are stored

     B = (a*U).B + (b*B) 
                  M                N                      N
   (           _______   )       _____        (         _____  )
  (           \ 0 1 3 |   )     | 0 3 |      (         | 0 3 |  )
 (  alpha* M    \ . . |    )  M | . . |  +  ( beta*  M | . . |   )
  (               \ . |   )     | . . |      (         | . . |  )
   (                --  )    x   -----        (         -----  )

Parameters
name         type           size         description
M	     const long     1            rows of U,A,B
N            const long     1            cols of A,B
ALPHA        const T        1            constant to scale U by
U            const T*       (M*(M+1))/2  pointer to matrix U    
A            const T*       M*N          pointer to matrix A
BETA         const T        1            constant to scale B by
B            T*             M*N          pointer to matrix B
-------------------------------------------------------------------------*/
#include "linal_UApB.hpp"

template <typename T>
void linal_UApB(const long M, const long N, const T ALPHA, const T* U, 
                const T* A,const T BETA, T* B)
{
  T TMP;
  const T* UP;
  const T* AP;
  T* BP;
  //ALPHA == 1, BETA == 0, common case
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {

    //loop over cols of B,A
    for (long j=0;j<N;j++)
    {
      BP = B + M*j; 
      AP = A + M*j;

      //loop over rows of B,U
      for (long i=0;i<M;i++)
      {
        UP = U + (i*(i+1))/2;

        //elements up to i in U are in order
        TMP = simd_dot<T>(i+1,UP,AP);

        //elements of U after i are out of order
        for (long k=i+1;k<M;k++)
        {
          TMP += *(U+(k*(k+1))/2+i) * *(AP+k);
        }

        *(BP+i) = TMP;

      }
    }

  //ALPHA == 1, BETA == 1, common case
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL ) {

    //loop over cols of B,A
    for (long j=0;j<N;j++)
    {
      BP = B + M*j; 
      AP = A + M*j;

      //loop over rows of B,U
      for (long i=0;i<M;i++)
      {
        UP = U + (i*(i+1))/2;

        //elements up to i in U are in order
        TMP = simd_dot<T>(i+1,UP,AP);

        //elements of U after i are out of order
        for (long k=i+1;k<M;k++)
        {
          TMP += *(U+(k*(k+1))/2+i) * *(AP+k);
        }

        *(BP+i) += TMP;

      }
    }

  //General case
  } else {

    //loop over cols of B,A
    for (long j=0;j<N;j++)
    {
      BP = B + M*j; 
      AP = A + M*j;

      //loop over rows of B,U
      for (long i=0;i<M;i++)
      {
        UP = U + (i*(i+1))/2;

        //elements up to i in U are in order
    
        TMP = simd_dot<T>(i+1,UP,AP);

        //elements of U after i are out of order
        for (long k=i+1;k<M;k++)
        {
          TMP += *(U+(k*(k+1))/2+i) * *(AP+k);
        }

        *(BP+i) = ALPHA*TMP + BETA * *(BP+i);

      }
    }

  } 


}
template void linal_UApB<double>(const long M, const long N, const double ALPHA, const double* U, const double* A, const double BETA, double* B);
template void linal_UApB<float>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);
template void linal_UApB<long>(const long M, const long N, const long ALPHA, const long* U, const long* A, const long BETA, long* B);
template void linal_UApB<int>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);


/*
template <typename T, const int ALIGN>
void linal_UApB(const long M, const long N, const T ALPHA, const T* U, 
                const T* A,const T BETA, T* B)
{
  T TMP;
  const T* UP;
  const T* AP;
  T* BP;
  //ALPHA == 1, BETA == 0, common case
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {

    //loop over cols of B,A
    for (long j=0;j<N;j++)
    {
      BP = B + M*j; 
      AP = A + M*j;

      //loop over rows of B,U
      for (long i=0;i<M;i++)
      {
        UP = U + (i*(i+1))/2;

        //elements up to i in U are in order
        TMP = simd_dot<T,ALIGN>(i+1,UP,AP);

        //elements of U after i are out of order
        for (long k=i+1;k<M;k++)
        {
          TMP += *(U+(k*(k+1))/2+i) * *(AP+k);
        }

        *(BP+i) = TMP;

      }
    }

  //ALPHA == 1, BETA == 1, common case
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL ) {

    //loop over cols of B,A
    for (long j=0;j<N;j++)
    {
      BP = B + M*j; 
      AP = A + M*j;

      //loop over rows of B,U
      for (long i=0;i<M;i++)
      {
        UP = U + (i*(i+1))/2;

        //elements up to i in U are in order
        TMP = simd_dot<T,ALIGN>(i+1,UP,AP);

        //elements of U after i are out of order
        for (long k=i+1;k<M;k++)
        {
          TMP += *(U+(k*(k+1))/2+i) * *(AP+k);
        }

        *(BP+i) += TMP;

      }
    }

  //General case
  } else {

    //loop over cols of B,A
    for (long j=0;j<N;j++)
    {
      BP = B + M*j; 
      AP = A + M*j;

      //loop over rows of B,U
      for (long i=0;i<M;i++)
      {
        UP = U + (i*(i+1))/2;

        //elements up to i in U are in order
    
        TMP = simd_dot<T,ALIGN>(i+1,UP,AP);

        //elements of U after i are out of order
        for (long k=i+1;k<M;k++)
        {
          TMP += *(U+(k*(k+1))/2+i) * *(AP+k);
        }

        *(BP+i) = ALPHA*TMP + BETA * *(BP+i);

      }
    }

  } 


}
template void linal_UApB<double,128>(const long M, const long N, const double ALPHA, const double* U, const double* A, const double BETA, double* B);
template void linal_UApB<double,64>(const long M, const long N, const double ALPHA, const double* U, const double* A, const double BETA, double* B);
template void linal_UApB<double,32>(const long M, const long N, const double ALPHA, const double* U, const double* A, const double BETA, double* B);
template void linal_UApB<double,16>(const long M, const long N, const double ALPHA, const double* U, const double* A, const double BETA, double* B);
template void linal_UApB<double,8>(const long M, const long N, const double ALPHA, const double* U, const double* A, const double BETA, double* B);

template void linal_UApB<float,128>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);
template void linal_UApB<float,64>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);
template void linal_UApB<float,32>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);
template void linal_UApB<float,16>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);
template void linal_UApB<float,8>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);
template void linal_UApB<float,4>(const long M, const long N, const float ALPHA, const float* U, const float* A, const float BETA, float* B);

template void linal_UApB<long,128>(const long M, const long N, const long ALPHA, const long* U, const long* A, const long BETA, long* B);
template void linal_UApB<long,64>(const long M, const long N, const long ALPHA, const long* U, const long* A, const long BETA, long* B);
template void linal_UApB<long,32>(const long M, const long N, const long ALPHA, const long* U, const long* A, const long BETA, long* B);
template void linal_UApB<long,16>(const long M, const long N, const long ALPHA, const long* U, const long* A, const long BETA, long* B);
template void linal_UApB<long,8>(const long M, const long N, const long ALPHA, const long* U, const long* A, const long BETA, long* B);

template void linal_UApB<int,128>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);
template void linal_UApB<int,64>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);
template void linal_UApB<int,32>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);
template void linal_UApB<int,16>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);
template void linal_UApB<int,8>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);
template void linal_UApB<int,4>(const long M, const long N, const int ALPHA, const int* U, const int* A, const int BETA, int* B);
*/
