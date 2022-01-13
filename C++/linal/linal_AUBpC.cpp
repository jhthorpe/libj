/*-----------------------------------------------------------
  linal_AUBpC.cpp
        JHT, January 7, 2022 : created

  .cpp file for AUBpC, which matrix multiplies a
  MxL matrix A (which may be scaled), 
  and LxL upper symmetric matrix U, a LxN matrix B, 
  and adds the result to a (scaled) MxN matrix C
  
  **IN PLACE ALGORITHM** 

  It is assumed that A,U,B, and C are held continuously 
  in memory, that A,U,B, and C  are stored column major, and that 
  only the upper triangular elements of U 
  
           A            U           B                    C
         _______    ________     _______              _______
        | 0 2 4 |   \ 0 1 3 |   | 0 3 6 |            | 0 2 4 |
alpha*  | . . . |     \ . . |   | . . . |  += beta * | . . . |
         _______        \ . |   | . . . |             -------
                  .      ---  .  -------             

This algorithm proceeds by constructing the intermediate elements of U.B,
 and distributing them along the rows of A

  This uses the following equations:

  C(i,j) = sum_k A(i,k)*I(k,j)
  I(k,j) = sum_l U(k,l)*B(l,j)

This could be improved by blocking, but we will do this at
a later date

Parameters:
M       const long      rows of A,C
N       const long      cols of B,C
L       const long      cols of A,rows of U,B
ALPHA   cosnt T         value to scale A with
A*      const T*        pointer to A
U*      const T*        pointer to U
B*      const T*        pointer to B
BETA    const T         value to scale C with
C*      T*              pointer to C


-----------------------------------------------------------*/
#include "linal_AUBpC.hpp"
#include <stdio.h>

template <typename T>
void linal_AUBpC(const long M, const long N, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* C)
{
  const long CLEN = M*N;
  const T* UP;
  const T* AP;
  const T* BP;
  T* CP; 
  T TMP;

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set C to zero
    simd_zero<T>(CLEN,C);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of C
      for (long j=0;j<N;j++)
      {
        CP = C + M*j;
        BP = B + L*j;

        //construct intermediates I(k,j)
        //the first k elements of U are linear in memory
        TMP = simd_dot<T>(k+1,UP,BP);
        //And here, we just have to desperately hope that
        // the out of order elements of U are temporally local
        for (long l=k+1;l<L;l++)
        {
          TMP += *(U+(l*(l+1))/2+k) * *(BP+l); 
        }

        //distribute along cols of C
        simd_axpy<T>(M,TMP,AP,CP);

      }//loop over j

    }//loop over k


  //common case, alpha == 1 beta == 1
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL ) {

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of C
      for (long j=0;j<N;j++)
      {
        CP = C + M*j;
        BP = B + L*j;

        //construct intermediates I(k,j)
        //the first k elements of U are linear in memory
        TMP = simd_dot<T>(k+1,UP,BP);
        //And here, we just have to desperately hope that
        // the out of order elements of U are temporally local
        for (long l=k+1;l<L;l++)
        {
          TMP += *(U+(l*(l+1))/2+k) * *(BP+l); 
        }

        //distribute along cols of C
        simd_axpy<T>(M,TMP,AP,CP);

      }//loop over j

    }//loop over k

  //General case
  } else {

    //scale C 
    simd_scal_mul<T>(CLEN,BETA,C);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of C
      for (long j=0;j<N;j++)
      {
        CP = C + M*j;
        BP = B + L*j;

        //construct intermediates I(k,j)
        //the first k elements of U are linear in memory
        TMP = simd_dot<T>(k+1,UP,BP);
        //And here, we just have to desperately hope that
        // the out of order elements of U are temporally local
        for (long l=k+1;l<L;l++)
        {
          TMP += *(U+(l*(l+1))/2+k) * *(BP+l); 
        }

        //distribute along cols of C
        simd_axpy<T>(M,ALPHA*TMP,AP,CP);

      }//loop over j

    }//loop over k

  }

}

template void linal_AUBpC<double>(const long M, const long N, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* C);
template void linal_AUBpC<float>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);
template void linal_AUBpC<long>(const long M, const long N, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* C);
template void linal_AUBpC<int>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);

/*
//Aligned code
template <typename T, const int ALIGN>
void linal_AUBpC(const long M, const long N, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* C)
{
  const long CLEN = M*N;
  const T* UP;
  const T* AP;
  const T* BP;
  T* CP; 
  T TMP;

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set C to zero
    simd_zero<T,ALIGN>(CLEN,C);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of C
      for (long j=0;j<N;j++)
      {
        CP = C + M*j;
        BP = B + L*j;

        //construct intermediates I(k,j)
        //the first k elements of U are linear in memory
        TMP = simd_dot<T,ALIGN>(k+1,UP,BP);
        //And here, we just have to desperately hope that
        // the out of order elements of U are temporally local
        for (long l=k+1;l<L;l++)
        {
          TMP += *(U+(l*(l+1))/2+k) * *(BP+l); 
        }

        //distribute along cols of C
        simd_axpy<T,ALIGN>(M,TMP,AP,CP);

      }//loop over j

    }//loop over k


  //common case, alpha == 1 beta == 1
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL ) {

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of C
      for (long j=0;j<N;j++)
      {
        CP = C + M*j;
        BP = B + L*j;

        //construct intermediates I(k,j)
        //the first k elements of U are linear in memory
        TMP = simd_dot<T,ALIGN>(k+1,UP,BP);
        //And here, we just have to desperately hope that
        // the out of order elements of U are temporally local
        for (long l=k+1;l<L;l++)
        {
          TMP += *(U+(l*(l+1))/2+k) * *(BP+l); 
        }

        //distribute along cols of C
        simd_axpy<T,ALIGN>(M,TMP,AP,CP);

      }//loop over j

    }//loop over k

  //General case
  } else {

    //scale C 
    simd_scal_mul<T,ALIGN>(CLEN,BETA,C);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of C
      for (long j=0;j<N;j++)
      {
        CP = C + M*j;
        BP = B + L*j;

        //construct intermediates I(k,j)
        //the first k elements of U are linear in memory
        TMP = simd_dot<T,ALIGN>(k+1,UP,BP);
        //And here, we just have to desperately hope that
        // the out of order elements of U are temporally local
        for (long l=k+1;l<L;l++)
        {
          TMP += *(U+(l*(l+1))/2+k) * *(BP+l); 
        }

        //distribute along cols of C
        simd_axpy<T,ALIGN>(M,ALPHA*TMP,AP,CP);

      }//loop over j

    }//loop over k

  }


}

template void linal_AUBpC<double,128>(const long M, const long N, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* C);
template void linal_AUBpC<double,64>(const long M, const long N, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* C);
template void linal_AUBpC<double,32>(const long M, const long N, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* C);
template void linal_AUBpC<double,16>(const long M, const long N, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* C);
template void linal_AUBpC<double,8>(const long M, const long N, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* C);

template void linal_AUBpC<float,128>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);
template void linal_AUBpC<float,64>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);
template void linal_AUBpC<float,32>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);
template void linal_AUBpC<float,16>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);
template void linal_AUBpC<float,8>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);
template void linal_AUBpC<float,4>(const long M, const long N, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* C);

template void linal_AUBpC<long,128>(const long M, const long N, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* C);
template void linal_AUBpC<long,64>(const long M, const long N, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* C);
template void linal_AUBpC<long,32>(const long M, const long N, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* C);
template void linal_AUBpC<long,16>(const long M, const long N, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* C);
template void linal_AUBpC<long,8>(const long M, const long N, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* C);

template void linal_AUBpC<int,128>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);
template void linal_AUBpC<int,64>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);
template void linal_AUBpC<int,32>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);
template void linal_AUBpC<int,16>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);
template void linal_AUBpC<int,8>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);
template void linal_AUBpC<int,4>(const long M, const long N, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* C);
*/
