#include "jblis.h"
#include <stdio.h>

//Kernal with AVX
#if defined (AVX)
#define UNROLL 16
void libj_dzerov_p_kernal(double* X_ptr)
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
void libj_dzerov_p_kernal(double* X_ptr)
{
  X_ptr[0] = (double) 0; 
  X_ptr[1] = (double) 0; 
  X_ptr[2] = (double) 0; 
  X_ptr[3] = (double) 0; 
  X_ptr[4] = (double) 0; 
  X_ptr[5] = (double) 0; 
}
#endif

//Open MP code
#if defined (OMP)
#define OMP_CHUNK 1
void libj_dzerov_p(const long N, double* X)
{

//  #pragma omp parallel for schedule(static,OMP_CHUNK) if (N > OMP_CHUNK*16)
  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  {
    for (long i=0;i<N-UNROLL;i+=UNROLL)
    {
      libj_dzerov_p_kernal(X+i); 
    }
  }

  for (long i=N-UNROLL;i<N;i++)
  {
    X[i] = (double) 0; 
  }

}

//Non-OMP code
#else
void libj_dzerov_p(const long N, double* X)
{
  long i;
  for (i=0;i<N-UNROLL;i+=UNROLL)
  {
    libj_dzerov_p_kernal(X+i); 
  }

  for (;i<N;i++)
  {
    X[i] = (double) 0; 
  }

}
#endif

