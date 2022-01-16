#include "jblis.h"
#include <stdio.h>

//Kernal with AVX
#if defined (AVX)
#define UNROLL 16
void libj_dsetv_p_kernal(const double* A, double* X_ptr)
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
void libj_dsetv_p_kernal(double* X_ptr)
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
#define OMP_CHUNK 1
void libj_dsetv_p(const long N, const double A, double* X, const long XINC)
{

  //Stride == 1 gets good code
  if (XINC == 1) 
  {
  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  {
    for (long i=0;i<N-UNROLL;i+=UNROLL)
    {
      libj_dsetv_p_kernal(&A,X+i); 
    }
  }

  for (long i=N-UNROLL;i<N;i++)
  {
    X[i] = (double) A; 
  }
  

  //general code for XINC != 1
  } else {
    #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
    {
      for (long i=0;i<N;i+=XINC)
      { 
        X[i] = (double) A;
      }
    }
  }

}

