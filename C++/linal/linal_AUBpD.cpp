/*-----------------------------------------------------------
  linal_AUBpD.cpp
        JHT, January 9, 2022 : created

  .cpp file for AUBpD, which matrix multiplies a
  MxL matrix A (which may be scaled), 
  and LxL upper symmetric matrix U, a LxN matrix B, 
  and adds the diagonal elements of the resulting matrix 
  to a MxM diagonal matrix D 
   
  
  **IN PLACE ALGORITHM** 

  **This proceeds by constructing U.B, then distributing A**
  **If A is smaller than B, another algorithm should be used**

  It is assumed that A,U,B, and D are held continuously 
  in memory, that A,U,B, and D are stored column major, that 
  only the upper triangular elements of U and the diagonal 
  elements of D are stored.

            A           U          B                  D
         _______    ________     _____              _____
        | 0 2 4 |   \ 0 1 3 |   | 0 3 |            | 0   |
alpha*  | . . . |     \ . . |   | . . |  += beta * |   . |
         _______        \ . |   | . . |             -----
                  .      ---  .  -----             

  
This algorithm proceeds by constructing the intermediate elements of U.B,
 and distributing them along the rows of A. 

  This uses the following equations:

  D(j,j) = sum_k A(i,k)*I(k,j)
  I(k,j) = sum_l U(k,l)*B(l,j)

This could be improved by blocking, but we will do this at
a later date

Parameters:
M       const long      rows of A,D, cols of B
L       const long      cols of A,rows of U,B
ALPHA   cosnt T         value to scale A with
A*      const T*        pointer to A
U*      const T*        pointer to U
B*      const T*        pointer to B
BETA    const T         value to scale D with
D*      T*              pointer to D


-----------------------------------------------------------*/
#include "linal_AUBpD.hpp"
#include <stdio.h>

template <typename T>
void linal_AUBpD(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* D)
{
  const long DLEN = M; 
  const T* UP;
  const T* AP;
  const T* BP;
  T* DP; 
  T TMP;

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set D to zero
    simd_zero<T>(DLEN,D);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of D (diagonal matrix)
      for (long j=0;j<M;j++)
      {
        DP = D + j; 
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

        //add to diagonal element of D
        *DP += TMP**(AP+j);

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

      //loop over cols of D
      for (long j=0;j<M;j++)
      {
        DP = D + j; 
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

        //add to diagonal element of D
        *DP += TMP**(AP+j);

      }//loop over j

    }//loop over k

  //General case
  } else {

    //scale D 
    simd_scal_mul<T>(DLEN,BETA,D);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of D
      for (long j=0;j<M;j++)
      {
        DP = D + j; 
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

        //Add to diagonal element of D
        *DP += ALPHA*TMP**(AP+j);

      }//loop over j

    }//loop over k

  }

}

template void linal_AUBpD<double>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* D);
template void linal_AUBpD<float>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);
template void linal_AUBpD<long>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* D);
template void linal_AUBpD<int>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);

/*
//Aligned code
template <typename T, const int ALIGN>
void linal_AUBpD(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* D)
{

  const long DLEN = M; 
  const T* UP;
  const T* AP;
  const T* BP;
  T* DP; 
  T TMP;

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set D to zero
    simd_zero<T,ALIGN>(DLEN,D);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of D (diagonal matrix)
      for (long j=0;j<M;j++)
      {
        DP = D + j; 
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

        //add to diagonal element of D
        *DP += TMP**(AP+j);

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

      //loop over cols of D
      for (long j=0;j<M;j++)
      {
        DP = D + j; 
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

        //add to diagonal element of D
        *DP += TMP**(AP+j);

      }//loop over j

    }//loop over k

  //General case
  } else {

    //scale D 
    simd_scal_mul<T,ALIGN>(DLEN,BETA,D);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of D
      for (long j=0;j<M;j++)
      {
        DP = D + j; 
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

        //Add to diagonal element of D
        *DP += ALPHA*TMP**(AP+j);

      }//loop over j

    }//loop over k

  }
}



template void linal_AUBpD<double,128>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* D);
template void linal_AUBpD<double,64>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* D);
template void linal_AUBpD<double,32>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* D);
template void linal_AUBpD<double,16>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* D);
template void linal_AUBpD<double,8>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* D);

template void linal_AUBpD<float,128>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);
template void linal_AUBpD<float,64>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);
template void linal_AUBpD<float,32>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);
template void linal_AUBpD<float,16>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);
template void linal_AUBpD<float,8>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);
template void linal_AUBpD<float,4>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* D);

template void linal_AUBpD<long,128>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* D);
template void linal_AUBpD<long,64>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* D);
template void linal_AUBpD<long,32>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* D);
template void linal_AUBpD<long,16>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* D);
template void linal_AUBpD<long,8>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* D);

template void linal_AUBpD<int,128>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);
template void linal_AUBpD<int,64>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);
template void linal_AUBpD<int,32>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);
template void linal_AUBpD<int,16>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);
template void linal_AUBpD<int,8>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);
template void linal_AUBpD<int,4>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* D);
*/
