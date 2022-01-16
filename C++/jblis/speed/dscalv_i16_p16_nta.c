#include "jblis.h"
#include <stdio.h>

//Kernal with AVX
#if defined (AVX)
#define UNROLL 16
void libj_scalv_kernel(const double* A, double* X_ptr)
{
  _mm_prefetch(X_ptr+16,_MM_HINT_NTA);
  const __m256d a_0_3 = _mm256_broadcast_sd(A); 
  __m256d x_0_3   = _mm256_loadu_pd(X_ptr+0);  
  __m256d x_4_7   = _mm256_loadu_pd(X_ptr+4);  
  __m256d x_8_11  = _mm256_loadu_pd(X_ptr+8);  
  __m256d x_12_15 = _mm256_loadu_pd(X_ptr+12);  

  x_0_3   = _mm256_mul_pd(x_0_3,a_0_3);
  x_4_7   = _mm256_mul_pd(x_4_7,a_0_3);
  x_8_11  = _mm256_mul_pd(x_8_11,a_0_3);
  x_12_15 = _mm256_mul_pd(x_12_15,a_0_3);

  _mm256_storeu_pd(X_ptr+0 ,x_0_3);
  _mm256_storeu_pd(X_ptr+4 ,x_4_7); 
  _mm256_storeu_pd(X_ptr+8 ,x_8_11); 
  _mm256_storeu_pd(X_ptr+12,x_12_15); 
}

/*
#define UNROLL 32
void libj_scalv_kernel(const double* A, double* X_ptr)
{
  const __m256d a_0_3 = _mm256_broadcast_sd(A); 
  __m256d x_0_3   = _mm256_loadu_pd(X_ptr+0);  
  __m256d x_4_7   = _mm256_loadu_pd(X_ptr+4);  
  __m256d x_8_11  = _mm256_loadu_pd(X_ptr+8);  
  __m256d x_12_15 = _mm256_loadu_pd(X_ptr+12);  
  __m256d x_16_19 = _mm256_loadu_pd(X_ptr+16);  
  __m256d x_20_23 = _mm256_loadu_pd(X_ptr+20);  
  __m256d x_24_27 = _mm256_loadu_pd(X_ptr+24);  
  __m256d x_28_31 = _mm256_loadu_pd(X_ptr+28);  

  x_0_3   = _mm256_mul_pd(x_0_3  ,a_0_3);
  x_4_7   = _mm256_mul_pd(x_4_7  ,a_0_3);
  x_8_11  = _mm256_mul_pd(x_8_11 ,a_0_3);
  x_12_15 = _mm256_mul_pd(x_12_15,a_0_3);
  x_16_19 = _mm256_mul_pd(x_16_19,a_0_3);
  x_20_23 = _mm256_mul_pd(x_20_23,a_0_3);
  x_24_27 = _mm256_mul_pd(x_24_27,a_0_3);
  x_28_31 = _mm256_mul_pd(x_28_31,a_0_3);

  _mm256_storeu_pd(X_ptr+0 ,x_0_3);
  _mm256_storeu_pd(X_ptr+4 ,x_4_7); 
  _mm256_storeu_pd(X_ptr+8 ,x_8_11); 
  _mm256_storeu_pd(X_ptr+12,x_12_15); 
  _mm256_storeu_pd(X_ptr+16,x_16_19); 
  _mm256_storeu_pd(X_ptr+20,x_20_23); 
  _mm256_storeu_pd(X_ptr+24,x_24_27); 
  _mm256_storeu_pd(X_ptr+28,x_28_31); 
}
*/

/*
#define UNROLL 64
void libj_scalv_kernel(const double* A, double* X_ptr)
{
  const __m256d a_0_3 = _mm256_broadcast_sd(A); 
  __m256d x_0_3   = _mm256_loadu_pd(X_ptr+0);  
  __m256d x_4_7   = _mm256_loadu_pd(X_ptr+4);  
  __m256d x_8_11  = _mm256_loadu_pd(X_ptr+8);  
  __m256d x_12_15 = _mm256_loadu_pd(X_ptr+12);  
  __m256d x_16_19 = _mm256_loadu_pd(X_ptr+16);  
  __m256d x_20_23 = _mm256_loadu_pd(X_ptr+20);  
  __m256d x_24_27 = _mm256_loadu_pd(X_ptr+24);  
  __m256d x_28_31 = _mm256_loadu_pd(X_ptr+28);  
  __m256d x_32_35 = _mm256_loadu_pd(X_ptr+32);  
  __m256d x_36_39 = _mm256_loadu_pd(X_ptr+36);  
  __m256d x_40_43 = _mm256_loadu_pd(X_ptr+40);  
  __m256d x_44_47 = _mm256_loadu_pd(X_ptr+44);  
  __m256d x_48_51 = _mm256_loadu_pd(X_ptr+48);  
  __m256d x_52_55 = _mm256_loadu_pd(X_ptr+52);  
  __m256d x_56_59 = _mm256_loadu_pd(X_ptr+56);  
  __m256d x_60_63 = _mm256_loadu_pd(X_ptr+60);  

  x_0_3   = _mm256_mul_pd(x_0_3,a_0_3);
  x_4_7   = _mm256_mul_pd(x_4_7,a_0_3);
  x_8_11  = _mm256_mul_pd(x_8_11,a_0_3);
  x_12_15 = _mm256_mul_pd(x_12_15,a_0_3);
  x_16_19 = _mm256_mul_pd(x_16_19,a_0_3);
  x_20_23 = _mm256_mul_pd(x_20_23,a_0_3);
  x_24_27 = _mm256_mul_pd(x_24_27,a_0_3);
  x_28_31 = _mm256_mul_pd(x_28_31,a_0_3);
  x_32_35 = _mm256_mul_pd(x_32_35,a_0_3);
  x_36_39 = _mm256_mul_pd(x_36_39,a_0_3);
  x_40_43 = _mm256_mul_pd(x_40_43,a_0_3);
  x_44_47 = _mm256_mul_pd(x_44_47,a_0_3);
  x_48_51 = _mm256_mul_pd(x_48_51,a_0_3);
  x_52_55 = _mm256_mul_pd(x_52_55,a_0_3);
  x_56_59 = _mm256_mul_pd(x_56_59,a_0_3);
  x_60_63 = _mm256_mul_pd(x_60_63,a_0_3);

  _mm256_storeu_pd(X_ptr+0 ,x_0_3);
  _mm256_storeu_pd(X_ptr+4 ,x_4_7); 
  _mm256_storeu_pd(X_ptr+8 ,x_8_11); 
  _mm256_storeu_pd(X_ptr+12,x_12_15); 
  _mm256_storeu_pd(X_ptr+16,x_16_19); 
  _mm256_storeu_pd(X_ptr+20,x_20_23); 
  _mm256_storeu_pd(X_ptr+24,x_24_27); 
  _mm256_storeu_pd(X_ptr+28,x_28_31); 
  _mm256_storeu_pd(X_ptr+32,x_32_35);
  _mm256_storeu_pd(X_ptr+36,x_36_39); 
  _mm256_storeu_pd(X_ptr+40,x_40_43); 
  _mm256_storeu_pd(X_ptr+44,x_44_47); 
  _mm256_storeu_pd(X_ptr+48,x_48_51); 
  _mm256_storeu_pd(X_ptr+52,x_52_55); 
  _mm256_storeu_pd(X_ptr+56,x_56_59); 
  _mm256_storeu_pd(X_ptr+60,x_60_63); 
}
*/
//Kernal if no AVX
#else
#define UNROLL 6
void libj_scalv_kernel(const double* A, double* X_ptr)
{
  X_ptr[0] *= A[0];
  X_ptr[1] *= A[0];
  X_ptr[2] *= A[0]; 
  X_ptr[3] *= A[0]; 
  X_ptr[4] *= A[0]; 
  X_ptr[5] *= A[0]; 
}
#endif

//Main function
void libj_dscalv(const long N, const double A, double* X, const long XINC)
{

  //Stride 1 has good code
  if (XINC == 1)
  {
    const long MOD = N % UNROLL;
    for (long i=0;i<MOD;i++)
    {
      X[i] *= A; 
    }
    
    for (long i=MOD;i<N;i+=UNROLL)
    {
      libj_scalv_kernel(&A,X+i); 
    }

  //General XINC does not
  } else {
    for (long i=0;i<N;i+=XINC)
    {
      X[i] *= A; 
    }
  }

  
}

