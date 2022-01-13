/*------------------------------------------------------------------------------
  linal_syMTM_UP_small
	JHT, August 20, 2021

  - a really badly coded version of the following operation:

    C = alpha*A^T.B + beta*C

    that only computes the upper triagular results of C. It is assumed that 
    A,B, and C are all continuous in memory, and stored column major


  TODO: consider loop unrolling/caching 
  
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
#include "linal_syMTM_UP_small.hpp"
template <typename T>
void linal_syMTM_UP_small(const int M,const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C)
{
  double dtmp;
  int aa,bb,cc=0,lim;
  
  //beta is zero, alpha is one (a common case)
  if (BETA == (double) 0 && ALPHA == (double) 1)
  {
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<=J;I++)
      {
        dtmp = 0;
        aa = K*I; //starting index of A column
        bb = K*J; //starting index of B column
        lim = aa+K-1;
        while (true)
        {
          dtmp += *(A+aa) * *(B+bb);
          if (aa >= lim) break;
          aa++;
          bb++; 
        }//inner loop, consider unrolling
        *(C+cc) = dtmp; 
        cc++; //consider caching this
      } //loop over I
    } //loop over J

  } else { //general case
    for (auto J=0;J<N;J++)
    {
      for (auto I=0;I<=J;I++)
      {
        dtmp = 0;
        aa = K*I; //starting index of A column
        bb = K*J; //starting index of B column
        lim = aa+K-1;
        while (true)
        {
          dtmp += *(A+aa) * *(B+bb);
          if (aa >= lim) break;
          aa++;
          bb++; 
        }//inner loop, consider unrolling
        *(C+cc) = BETA * *(C+cc) + ALPHA*dtmp; 
        cc++; //consider caching this?
      } //loop over I
    } //loop over J

  }//end if statements 
}

template void linal_syMTM_UP_small<double>(const int M,const int N,const int K,const double ALPHA, double* A, 
                                double* B,const double BETA, double* C);
template void linal_syMTM_UP_small<float>(const int M,const int N,const int K,const float ALPHA, float* A, 
                                float* B,const float BETA, float* C);
template void linal_syMTM_UP_small<int>(const int M,const int N,const int K,const int ALPHA, int* A, 
                                int* B,const int BETA, int* C);
template void linal_syMTM_UP_small<long>(const int M,const int N,const int K,const long ALPHA, long* A, 
                                long* B,const long BETA, long* C);

