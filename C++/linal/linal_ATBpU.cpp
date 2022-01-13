/*------------------------------------------------------------------------------
  linal_ATBpU
	JHT, December 8, 2021 : created 
	JHT, January 2, 2022 : renamed with new naming scheme

    C = alpha*A^T.B + beta*C

    Only computes the upper triagular results of C. It is assumed that 
    A,B, and C are all continuous in memory, and stored column major. 

    NOTE : C is assumed to be continous and **ONLY** stores the upper triangular
           elements

    This uses the simd library for the inner dot product.

    This is a important subroutine, and probably should be coded using
    M.M blocking algorithms

------------------------------------------------------------------------------*/
/* Variables

M	int	rows of A^T (cols of A), rows of C
N	int	cols of B, cols of C
K	int	cols of A^T (rows of A), rows of B, "internal" dimension
ALPHA	T	constant to multiply A by
A	T*	pointer to A
B	T*	pointer to B
BETA	T	constant to multiply C add by
C	T*	pointer to C (stores only uppertriangular elements)

*/
#include "linal_ATBpU.hpp"

//unaligned code
template <typename T>
void linal_ATBpU(const int M,const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C)
{
  long cc = 0;
  //beta is zero, alpha is one (a common case)
  if (BETA == (T) 0 && ALPHA == (T) 1)
  {
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<=J;I++)
      {

        *(C+cc) = simd_dot<T>(K,(A+K*I),(B+K*J));
        cc++; 

      } //loop over I
    } //loop over J

  } else { //general case
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<=J;I++)
      {

        *(C+cc) = BETA * *(C+cc) + ALPHA*simd_dot<T>(K,(A+K*I),(B+K*J)); 
        cc++; 

      } //loop over I
    } //loop over J

  }//end if statements 
}

template void linal_ATBpU<double>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                double* B,const double BETA, double* C);
template void linal_ATBpU<float>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                float* B,const float BETA, float* C);
template void linal_ATBpU<long>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                long* B,const long BETA, long* C);
template void linal_ATBpU<int>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                int* B,const int BETA, int* C);

/*
template <typename T, const int ALIGN>
void linal_ATBpU(const int M,const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C)
{
  long cc = 0;
  //beta is zero, alpha is one (a common case)
  if (BETA == (T) 0 && ALPHA == (T) 1)
  {
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<=J;I++)
      {

        *(C+cc) = simd_dot<T,ALIGN>(K,(A+K*I),(B+K*J));
        cc++; 

      } //loop over I
    } //loop over J

  } else { //general case
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<=J;I++)
      {

        *(C+cc) = BETA * *(C+cc) + ALPHA*simd_dot<T,ALIGN>(K,(A+K*I),(B+K*J)); 
        cc++; 

      } //loop over I
    } //loop over J

  }//end if statements 
}

template void linal_ATBpU<double,64>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ATBpU<double,32>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ATBpU<double,16>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                      double* B,const double BETA, double* C);
template void linal_ATBpU<double,8>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                     double* B,const double BETA, double* C);

template void linal_ATBpU<float,64>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ATBpU<float,32>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ATBpU<float,16>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                     float* B,const float BETA, float* C);
template void linal_ATBpU<float,8>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                    float* B,const float BETA, float* C);
template void linal_ATBpU<float,4>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                    float* B,const float BETA, float* C);

template void linal_ATBpU<long,64>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ATBpU<long,32>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ATBpU<long,16>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                    long* B,const long BETA, long* C);
template void linal_ATBpU<long,8>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                   long* B,const long BETA, long* C);

template void linal_ATBpU<int,64>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ATBpU<int,32>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ATBpU<int,16>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                   int* B,const int BETA, int* C);
template void linal_ATBpU<int,8>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                  int* B,const int BETA, int* C);
template void linal_ATBpU<int,4>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                  int* B,const int BETA, int* C);
*/
