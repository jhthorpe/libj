/*-----------------------------------------------------------
  linal_AUBpY.cpp
        JHT, January 7, 2022 : created

  .cpp file for AUBpY, which matrix multiplies a
  MxL matrix A (which may be scaled), 
  and LxL upper symmetric matrix U, a LxN matrix B, 
  and adds the result to a (scaled) MxM upper triangular matrix Y
  
  **IN PLACE ALGORITHM** 

  It is assumed that A,U,B, and Y are held continuously 
  in memory, that A,U,B, and Y are stored column major, and that 
  only the upper triangular elements of U and Y are stored.

            A           U          B                  Y
         _______    ________     _____             ______
        | 0 2 4 |   \ 0 1 3 |   | 0 3 |            \ 0 1 |
alpha*  | . . . |     \ . . |   | . . |  += beta *   \ . |
         _______        \ . |   | . . |               ---
                  .      ---  .  -----             

  
This algorithm proceeds by constructing the intermediate elements of U.B,
 and distributing them along the rows of A

  This uses the following equations:

  Y(i,j) = sum_k A(i,k)*I(k,j)
  I(k,j) = sum_l U(k,l)*B(l,j)

This could be improved by blocking, but we will do this at
a later date

Parameters:
M       const long      rows of A,Y, cols of B
L       const long      cols of A,rows of U,B
ALPHA   cosnt T         value to scale A with
A*      const T*        pointer to A
U*      const T*        pointer to U
B*      const T*        pointer to B
BETA    const T         value to scale Y with
Y*      T*              pointer to Y


-----------------------------------------------------------*/
#include "linal_AUBpY.hpp"
#include <stdio.h>

template <typename T>
void linal_AUBpY(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* Y)
{
  const long YLEN = (M*(M+1))/2;
  const T* UP;
  const T* AP;
  const T* BP;
  T* YP; 
  T TMP;

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set Y to zero
    simd_zero<T>(YLEN,Y);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of Y
      for (long j=0;j<M;j++)
      {
        YP = Y + (j*(j+1))/2;
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

        //distribute along cols of Y, down to diagonal
        simd_axpy<T>(j+1,TMP,AP,YP);

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

      //loop over cols of Y
      for (long j=0;j<M;j++)
      {
        YP = Y + (j*(j+1))/2;
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

        //distribute along cols of Y, down to diagonal
        simd_axpy<T>(j+1,TMP,AP,YP);

      }//loop over j

    }//loop over k

  //General case
  } else {

    //scale Y 
    simd_scal_mul<T>(YLEN,BETA,Y);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of Y
      for (long j=0;j<M;j++)
      {
        YP = Y + (j*(j+1))/2;
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

        //distribute along cols of Y, down to diagonal
        simd_axpy<T>(j+1,ALPHA*TMP,AP,YP);

      }//loop over j

    }//loop over k

  }

}

template void linal_AUBpY<double>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* Y);
template void linal_AUBpY<float>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);
template void linal_AUBpY<long>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* Y);
template void linal_AUBpY<int>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);

/*
//Aligned code
template <typename T, const int ALIGN>
void linal_AUBpY(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* Y)
{
  const long YLEN = (M*(M+1))/2;
  const T* UP;
  const T* AP;
  const T* BP;
  T* YP; 
  T TMP;

  printf("\nTESTING TESTING @ linal_AUBpY line 189\n");
  printf("M = %ld , L = %ld\n",M,L);
  printf("A start @ %ld , A end @ %ld\n",(long) A, (long) (A+M*L));
  printf("Y start @ %ld , Y end @ %ld\n",(long) Y, (long) (Y+YLEN));

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set Y to zero
    simd_zero<T,ALIGN>(YLEN,Y);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of Y
      for (long j=0;j<M;j++)
      {
        YP = Y + (j*(j+1))/2;
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

        //distribute along cols of Y, down to diagonal
        printf("\n@ line 224\n");
        printf("k=%ld, j=%ld\n",k,j);
        printf("AP start @ %ld , AP end at %ld\n",(long)(AP),(long)(AP+j));
        printf("YP start @ %ld , YP end at %ld\n",(long)(YP),(long)(YP+j));
        simd_axpy<T,ALIGN>(j+1,TMP,AP,YP);

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

      //loop over cols of Y
      for (long j=0;j<M;j++)
      {
        YP = Y + (j*(j+1))/2;
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

        //distribute along cols of Y, down to diagonal
        simd_axpy<T,ALIGN>(j+1,TMP,AP,YP);

      }//loop over j

    }//loop over k

  //General case
  } else {

    //scale Y 
    simd_scal_mul<T,ALIGN>(YLEN,BETA,Y);

    //loop over k, cols of A
    for (long k=0;k<L;k++)
    {
      AP = A + M*k;
      UP = U + (k*(k+1))/2;

      //loop over cols of Y
      for (long j=0;j<M;j++)
      {
        YP = Y + (j*(j+1))/2;
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

        //distribute along cols of Y, down to diagonal
        //Go to first aligned element
        simd_axpy<T,ALIGN>(j+1,ALPHA*TMP,AP,YP);

      }//loop over j

    }//loop over k

  }

}



template void linal_AUBpY<double,128>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* Y);
template void linal_AUBpY<double,64>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* Y);
template void linal_AUBpY<double,32>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* Y);
template void linal_AUBpY<double,16>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* Y);
template void linal_AUBpY<double,8>(const long M, const long L, const double ALPHA, const double* A, const double* U, const double* B, const double BETA, double* Y);

template void linal_AUBpY<float,128>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);
template void linal_AUBpY<float,64>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);
template void linal_AUBpY<float,32>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);
template void linal_AUBpY<float,16>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);
template void linal_AUBpY<float,8>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);
template void linal_AUBpY<float,4>(const long M, const long L, const float ALPHA, const float* A, const float* U, const float* B, const float BETA, float* Y);

template void linal_AUBpY<long,128>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* Y);
template void linal_AUBpY<long,64>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* Y);
template void linal_AUBpY<long,32>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* Y);
template void linal_AUBpY<long,16>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* Y);
template void linal_AUBpY<long,8>(const long M, const long L, const long ALPHA, const long* A, const long* U, const long* B, const long BETA, long* Y);

template void linal_AUBpY<int,128>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);
template void linal_AUBpY<int,64>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);
template void linal_AUBpY<int,32>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);
template void linal_AUBpY<int,16>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);
template void linal_AUBpY<int,8>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);
template void linal_AUBpY<int,4>(const long M, const long L, const int ALPHA, const int* A, const int* U, const int* B, const int BETA, int* Y);
*/
