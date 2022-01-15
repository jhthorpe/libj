#include "jblis.h"
#include <stdio.h>

//Kernal with AVX
#if defined (AVX)
#define UNROLL 16
void libj_zerov_kernel(const double a, double* X_ptr)
{
  const __m256d z03 = _mm256_setzero_pd(); 
  _mm256_storeu_pd(X_ptr+0,z03); 
  _mm256_storeu_pd(X_ptr+4,z03); 
  _mm256_storeu_pd(X_ptr+8,z03); 
  _mm256_storeu_pd(X_ptr+12,z03); 
}

//Kernal if no AVX
#else
#define UNROLL 6
void libj_zerov_kernel(const double a, double* X_ptr)
{
  X_ptr[0] = (double) a; 
  X_ptr[1] = (double) a; 
  X_ptr[2] = (double) a; 
  X_ptr[3] = (double) a; 
  X_ptr[4] = (double) a; 
  X_ptr[5] = (double) a; 
}
#endif

//Main function
void libj_dsetv(const long N, const double a, double* X)
{
  long i;
  for (i=0;i<N-UNROLL;i+=UNROLL)
  {
    libj_zerov_kernel(a,X+i); 
  }

  for (;i<N;i++)
  {
    X[i] = (double) a; 
  }
}

