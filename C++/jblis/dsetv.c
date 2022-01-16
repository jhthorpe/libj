#include "jblis.h"
#include <stdio.h>

//Kernal with AVX
#if defined (AVX)
#define UNROLL 16
void libj_setv_kernel(const double* A, double* X_ptr)
{
  const __m256d a03 = _mm256_broadcast_sd(A); 
  _mm256_storeu_pd(X_ptr+0,a03); 
  _mm256_storeu_pd(X_ptr+4,a03); 
  _mm256_storeu_pd(X_ptr+8,a03); 
  _mm256_storeu_pd(X_ptr+12,a03); 
}

//Kernal if no AVX
#else
#define UNROLL 6
void libj_setv_kernel(const double A, double* X_ptr)
{
  X_ptr[0] = (double) A; 
  X_ptr[1] = (double) A; 
  X_ptr[2] = (double) A; 
  X_ptr[3] = (double) A; 
  X_ptr[4] = (double) A; 
  X_ptr[5] = (double) A; 
}
#endif

//Main function
void libj_dsetv(const long N, const double A, double* X, const long XINC)
{

  //Stride 1 has good code
  if (XINC == 1)
  {

    const long MOD = N % UNROLL;
    for (long i=0;i<MOD;i++)
    {
      X[i] = (double) A; 
    }
    
    for (long i=MOD;i<N;i+=UNROLL)
    {
      libj_setv_kernel(&A,X+i); 
    }

  //General XINC does not
  } else {
    for (long i=0;i<N;i+=XINC)
    {
      X[i] = (double) A;
    }
  }

  
}

