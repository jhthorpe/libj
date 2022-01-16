#include "jblis.h"
#include <stdio.h>

//Kernal with AVX
#if defined (AVX)
#define UNROLL 16
void libj_zerov_kernel(double* X_ptr)
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
void libj_zerov_kernel(double* X_ptr)
{
  X_ptr[0] = (double) 0; 
  X_ptr[1] = (double) 0; 
  X_ptr[2] = (double) 0; 
  X_ptr[3] = (double) 0; 
  X_ptr[4] = (double) 0; 
  X_ptr[5] = (double) 0; 
}
#endif

//Main function
void libj_dzerov(const long N, double* X, const long XINC)
{

  //Stride 1 has good code
  if (XINC == 1)
  {

    const long MOD = N % UNROLL;
    for (long i=0;i<MOD;i++)
    {
      X[i] = (double) 0; 
    }
    
    for (long i=MOD;i<N;i+=UNROLL)
    {
      libj_zerov_kernel(X+i); 
    }


  //General XINC does not
  } else {
    for (long i=0;i<N;i+=XINC)
    {
      X[i] = (double) 0;
    }
  }

  
}

