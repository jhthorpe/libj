/*----------------------------------------------------------------------
  axpby.cpp
	JHT, April 11, 2022 : created

  .cpp file for the axpby function, which performs

  Y = a*X + b*Y 
----------------------------------------------------------------------*/
#include "jblis.hpp"

//Definitions
#define OMP_CHUNK 1024
#if defined (AVX) || (FMA)
  #define UNROLL 8
#else
  #define UNROLL 4
#endif

//General code
#if defined (AVX) || defined (FMA) 
template <typename T>
void axpby_kernal(const T a, const T* X_ptr, const T b, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*X_ptr[0] + b*Y_ptr[0]; 
  Y_tmp[1] = a*X_ptr[1] + b*Y_ptr[1]; 
  Y_tmp[2] = a*X_ptr[2] + b*Y_ptr[2]; 
  Y_tmp[3] = a*X_ptr[3] + b*Y_ptr[3]; 
  Y_tmp[4] = a*X_ptr[4] + b*Y_ptr[4]; 
  Y_tmp[5] = a*X_ptr[5] + b*Y_ptr[5]; 
  Y_tmp[6] = a*X_ptr[6] + b*Y_ptr[6]; 
  Y_tmp[7] = a*X_ptr[7] + b*Y_ptr[7]; 

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
  Y_ptr[4] = Y_tmp[4];
  Y_ptr[5] = Y_tmp[5];
  Y_ptr[6] = Y_tmp[6];
  Y_ptr[7] = Y_tmp[7];
}
template void axpby_kernal<float>(const float a, const float* X_ptr, 
                                  const float b, float* Y_ptr); 
template void axpby_kernal<long>(const long a, const long* X_ptr,
                                 const long b, long* Y_ptr); 
template void axpby_kernal<int>(const int a, const int* X_ptr,
                                const int b, int* Y_ptr); 

//General code without AVX
#else
template <typename T>
void axpby_kernal(const T a, const T* X_ptr, const T b, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*X_ptr[0] + b*Y_ptr[0];
  Y_tmp[1] = a*X_ptr[1] + b*Y_ptr[1];
  Y_tmp[2] = a*X_ptr[2] + b*Y_ptr[2];
  Y_tmp[3] = a*X_ptr[3] + b*Y_ptr[3];

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
}
template void axpby_kernal<double>(const double a, const double* X_ptr,
                                   const double b, double* Y_ptr); 
template void axpby_kernal<float>(const float* a, const float* X_ptr,
                                  const float b, float* Y_ptr); 
template void axpby_kernal<long>(const long a, const long* X_ptr,
                                 const long b, long* Y_ptr); 
template void axpby_kernal<int>(const int a, const int* X_ptr,
                                const int b, int* Y_ptr); 
#endif 


//Specialized code for doubles
#if defined (FMA)
template<>
void axpby_kernal(const double a, const double* X_ptr, const double b, double* Y_ptr)
{
  _mm_prefetch(Y_ptr+0, _MM_HINT_T0);
  _mm_prefetch(X_ptr+0, _MM_HINT_T0);

  const __m256d b_0_3 = _mm256_broadcast_sd(&b);
  __m256d Y_0_3 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+0)); 
  __m256d Y_4_7 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+4)); 

  const __m256d a_0_3 = _mm256_broadcast_sd(&a);
  Y_0_3 = _mm256_fmadd_pd(a_0_3,_mm256_loadu_pd(X_ptr+0),Y_0_3);
  Y_4_7 = _mm256_fmadd_pd(a_0_3,_mm256_loadu_pd(X_ptr+4),Y_4_7);

  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}

#elif defined (AVX)
//specialized template for double with AVX
template<>
void axpby_kernal(const double a, const double* X_ptr, const double b, double* Y_ptr)
{
  _mm_prefetch(Y_ptr+0, _MM_HINT_T0);
  _mm_prefetch(X_ptr+0, _MM_HINT_T0);

  const __m256d a_0_3 = _mm256_broadcast_sd(&a);
  const __m256d X_0_3 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+0));
  const __m256d X_4_7 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+4));

  const __m256d b_0_3 = _mm256_broadcast_sd(&b);
  __m256d Y_0_3 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+0));
  __m256d Y_4_7 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+4));

  Y_0_3 = _mm256_add_pd(Y_0_3,X_0_3);
  Y_4_7 = _mm256_add_pd(Y_4_7,X_4_7);
 
  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}
#endif

//General code around kernal
template <typename T>
void libj::axpby(const T a, const libj::tensor<T>& X, const T b, libj::tensor<T>& Y)
{
  //check for shape
  if (!same_shape(X,Y)) 
  {
    printf("ERROR libj::axpby tensors X and Y do not have the same shape\n");
    exit(1);
  }

  //check for simple cases
  if (a == (T) 1 && b == (T) 1)
  {
    libj::xpy(X,Y); return;
  } else if (a != (T) 1 && b == (T) 1) {
    libj::axpy(a,X,Y); return;
  } else if (a == (T) 1 && b != (T) 1) {
    libj::xpby(X,b,Y); return;
  } else if (a == (T) 0 && b != (T) 0) {
    libj::scal(b,Y);
  } else if (a != (T) 0 && b == (T) 0) {
    libj::scopy(a,X,Y);
  } else if (a == (T) 0 && b == (T) 0) {
    libj::zero(Y);
  } 

  //General axpby loops
  const long N = X.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    Y[i] = a*X[i] + b*Y[i]; 
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    axpby_kernal<T>(a,&X[i],b,&Y[i]);
  }

}
template void libj::axpby<double>(const double a, const libj::tensor<double>& X,
                                  const double b, libj::tensor<double>& Y);
template void libj::axpby<float>(const float a, const libj::tensor<float>& X,
                                 const float b, libj::tensor<float>& Y);
template void libj::axpby<long>(const long a, const libj::tensor<long>& X,
                                const long b, libj::tensor<long>& Y);
template void libj::axpby<int>(const int a, const libj::tensor<int>& X,
                               const int b, libj::tensor<int>& Y);


