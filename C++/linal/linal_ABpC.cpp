/*------------------------------------------------
  linal_ABpC.cpp
        JHT, December 8, 2021 : created 

    C = ALPHA*A.B + BETA*C  

    It is assumed that C,A,and B are all 
    continous in memory, and stored column 
    major (logical dimension == physical dimension) 

    Currently this is implemented using 
    one vector of A at a time to distribute 
    along the cols of C
------------------------------------------------*/

/* Variables

M	int	rows of A^T (cols of A), rows of C
N	int	cols of B, cols of C
K	int	cols of A^T (rows of A), rows of B, "internal" dimension
ALPHA	T	constant to scale A by
A	T*	pointer to A
B	T*	pointer to B
BETA	T	constant to scale C by
C	T*	pointer to C 

*/
#include "linal_ABpC.hpp"

//unaligned code
template <typename T>
void linal_ABpC(const int M, const int N, const int K, const T ALPHA, T* A, T* B, const T BETA, T* C)
{
  T* cc;
  //BETA is zero, ALPHA is one (a common case)
  if (fabs((double)ALPHA - (double) 1) < DZTOL
   && fabs((double) BETA) < DZTOL)
  {
    //loop over cols of C
    for (auto J=0;J<N;J++)
    {

      cc = C+M*J; //column of C we're working on

      //zero the column
      simd_zero<T>(M,cc); 
      
      //loop through the other cols of A and down col of B 
      for (auto I=0;I<K;I++)
      {
        simd_axpy<T>(M,*(B+K*J+I),(A+M*I),cc); 
      }
    } 

  //General case
  } else {

    //loop over cols of C
    for (auto J=0;J<N;J++)
    {

      cc = C+M*J; //column of C we're working on

      //+BETA*C for this column
      simd_axpy<T>(M,BETA,cc,cc); 
      
      //loop through the other cols of A and down col of B 
      for (auto I=0;I<K;I++)
      {
        simd_axpy<T>(M,ALPHA**(B+K*J+I),(A+M*I),cc); 
      }
    } 

  }//end if statements 
}

template void linal_ABpC<double>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                double* B,const double BETA, double* C);
template void linal_ABpC<float>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                float* B,const float BETA, float* C);
template void linal_ABpC<long>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                long* B,const long BETA, long* C);
template void linal_ABpC<int>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                int* B,const int BETA, int* C);
/*
template <typename T, const int ALIGN>
void linal_ABpC(const int M,const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C)
{
  T* cc;
  //BETA is zero, ALPHA is one (a common case)
  if (fabs((double)ALPHA - (double) 1) < DZTOL
   && fabs((double) BETA) < DZTOL)
  {
    //loop over cols of C
    for (auto J=0;J<N;J++)
    {

      cc = C+M*J; //column of C we're working on

      //zero the column
      simd_zero<T,ALIGN>(M,cc); 
      
      //loop through the other cols of A and down col of B 
      for (auto I=0;I<K;I++)
      {
        simd_axpy<T,ALIGN>(M,*(B+K*J+I),(A+M*I),cc); 
      }
    } 

  //General case
  } else {

    //loop over cols of C
    for (auto J=0;J<N;J++)
    {

      cc = C+M*J; //column of C we're working on

      //+BETA*C for this column
      simd_axpy<T,ALIGN>(M,BETA,cc,cc); 
      
      //loop through the other cols of A and down col of B 
      for (auto I=0;I<K;I++)
      {
        simd_axpy<T,ALIGN>(M,ALPHA**(B+K*J+I),(A+M*I),cc); 
      }
    } 

  }//end if statements 
}

template void linal_ABpC<double,64>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ABpC<double,32>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ABpC<double,16>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ABpC<double,8>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                     double* B,const double BETA, double* C);

template void linal_ABpC<float,64>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ABpC<float,32>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ABpC<float,16>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ABpC<float,8>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                    float* B,const float BETA, float* C);
template void linal_ABpC<float,4>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                    float* B,const float BETA, float* C);

template void linal_ABpC<long,64>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ABpC<long,32>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ABpC<long,16>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ABpC<long,8>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                   long* B,const long BETA, long* C);

template void linal_ABpC<int,64>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ABpC<int,32>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ABpC<int,16>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ABpC<int,8>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                  int* B,const int BETA, int* C);
template void linal_ABpC<int,4>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                  int* B,const int BETA, int* C);
*/
