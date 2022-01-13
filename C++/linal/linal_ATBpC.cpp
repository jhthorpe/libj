/*------------------------------------------------
  linal_ATBpC.cpp
        JHT, December 8, 2021 : created 

    C = alpha*A^T.B + beta*C  

    It is assumed that C,A,and B are all 
    continous in memory, and stored column 
    major (logical dimension == physical dimension) 
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
#include "linal_ATBpC.hpp"

//unaligned code
template <typename T>
void linal_ATBpC(const int M, const int N, const int K, const T ALPHA, T* A, T* B, const T BETA, T* C)
{
  long cc = 0;
  //BETA is zero, ALPHA is one (a common case)
  if (fabs((double)ALPHA - (double) 1) < DZTOL
   && fabs((double) BETA) < DZTOL)
  {
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<M;I++)
      {

        *(C+cc) = simd_dot<T>(K,(A+K*I),(B+K*J));
        cc++; 

      } //loop over I
    } //loop over J

  } else { //general case
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<M;I++)
      {

        *(C+cc) = BETA * *(C+cc) + ALPHA*simd_dot<T>(K,(A+K*I),(B+K*J)); 
        cc++; 

      } //loop over I
    } //loop over J

  }//end if statements 
}

template void linal_ATBpC<double>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                double* B,const double BETA, double* C);
template void linal_ATBpC<float>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                float* B,const float BETA, float* C);
template void linal_ATBpC<long>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                long* B,const long BETA, long* C);
template void linal_ATBpC<int>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                int* B,const int BETA, int* C);

/*
template <typename T, const int ALIGN>
void linal_ATBpC(const int M,const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C)
{
  long cc = 0;
  //BETA is zero, ALPHA is one (a common case)
  if (BETA == (T) 0 && ALPHA == (T) 1)
  {
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<M;I++)
      {

        *(C+cc) = simd_dot<T,ALIGN>(K,(A+K*I),(B+K*J));
        cc++; 

      } //loop over I
    } //loop over J

  } else { //general case
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<M;I++)
      {

        *(C+cc) = BETA * *(C+cc) + ALPHA*simd_dot<T,ALIGN>(K,(A+K*I),(B+K*J)); 
        cc++; 

      } //loop over I
    } //loop over J

  }//end if statements 
}

template void linal_ATBpC<double,64>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ATBpC<double,32>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ATBpC<double,16>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ATBpC<double,8>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                     double* B,const double BETA, double* C);

template void linal_ATBpC<float,64>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ATBpC<float,32>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ATBpC<float,16>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ATBpC<float,8>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                    float* B,const float BETA, float* C);
template void linal_ATBpC<float,4>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                    float* B,const float BETA, float* C);

template void linal_ATBpC<long,64>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ATBpC<long,32>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ATBpC<long,16>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ATBpC<long,8>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                   long* B,const long BETA, long* C);

template void linal_ATBpC<int,64>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ATBpC<int,32>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ATBpC<int,16>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ATBpC<int,8>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                  int* B,const int BETA, int* C);
template void linal_ATBpC<int,4>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                  int* B,const int BETA, int* C);
*/
