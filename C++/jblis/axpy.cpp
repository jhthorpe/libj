/*----------------------------------------------------------------------
  axpy.cpp
	JHT, April 11, 2022 : created

  .cpp file for the axpy function, which performs

  Y = a*X + Y

----------------------------------------------------------------------*/
#include "jblis.hpp"

//Definitions
#define OMP_CHUNK 1024
#if defined (AVX) || defined (FMA)
  #define UNROLL 8
#else
  #define UNROLL 4
#endif

//General code
#if defined (AVX) || defined (FMA)
template <typename T>
void axpy_kernal(const T a, const T* X_ptr, T* Y_ptr)
{ 
  T Y_tmp[UNROLL] = {}; 
  Y_tmp[0] = a*X_ptr[0] + Y_ptr[0];
  Y_tmp[1] = a*X_ptr[1] + Y_ptr[1];
  Y_tmp[2] = a*X_ptr[2] + Y_ptr[2];
  Y_tmp[3] = a*X_ptr[3] + Y_ptr[3];
  Y_tmp[4] = a*X_ptr[4] + Y_ptr[4];
  Y_tmp[5] = a*X_ptr[5] + Y_ptr[5];
  Y_tmp[6] = a*X_ptr[6] + Y_ptr[6];
  Y_tmp[7] = a*X_ptr[7] + Y_ptr[7];
  
  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
  Y_ptr[4] = Y_tmp[4];
  Y_ptr[5] = Y_tmp[5];
  Y_ptr[6] = Y_tmp[6];
  Y_ptr[7] = Y_tmp[7];
}
template void axpy_kernal<float>(const float a, const float* X_ptr,
                                 float* Y_ptr);
template void axpy_kernal<long>(const long a, const long* X_ptr,
                                long* Y_ptr);
template void axpy_kernal<int>(const int a, const int* X_ptr,
                               int* Y_ptr);

#else

template <typename T>
void axpy_kernal(const T a, const T* X_ptr, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*X_ptr[0] + Y_ptr[0];
  Y_tmp[1] = a*X_ptr[1] + Y_ptr[1];
  Y_tmp[2] = a*X_ptr[2] + Y_ptr[2];
  Y_tmp[3] = a*X_ptr[3] + Y_ptr[3];

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
}
template void axpy_kernal<double>(const double a, const double* X_ptr,
                                  double* Y_ptr); 
template void axpy_kernal<float>(const float a, const float* X_ptr,
                                 float* Y_ptr); 
template void axpy_kernal<long>(const long a, const long* X_ptr,
                                long* Y_ptr); 
template void axpy_kernal<int>(const int a, const int* X_ptr,
                               int* Y_ptr); 
#endif

#if defined (FMA)
//Specialized template for double with FMA
template<>
void axpy_kernal(const double a, const double* X_ptr, double* Y_ptr)
{
  //I can see several ways of ordering this...

  _mm_prefetch(Y_ptr+64, _MM_HINT_T2);
  _mm_prefetch(X_ptr+64, _MM_HINT_T2);

  const __m256d a_0_3   = _mm256_broadcast_sd(&a);

  const __m256d Y_0_3   = _mm256_fmadd_pd(a_0_3,_mm256_loadu_pd(X_ptr+0),
                                                _mm256_loadu_pd(Y_ptr+0));
  const __m256d Y_4_7   = _mm256_fmadd_pd(a_0_3,_mm256_loadu_pd(X_ptr+4),
                                                _mm256_loadu_pd(Y_ptr+4));
//  const __m256d Y_8_11  = _mm256_fmadd_pd(a_0_3,_mm256_loadu_pd(X_ptr+8),
//                                                _mm256_loadu_pd(Y_ptr+8));
//  const __m256d Y_12_15 = _mm256_fmadd_pd(a_0_3,_mm256_loadu_pd(X_ptr+12),
//                                                _mm256_loadu_pd(Y_ptr+12));

  _mm256_storeu_pd(Y_ptr+0  ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4  ,Y_4_7);
//  _mm256_storeu_pd(Y_ptr+8  ,Y_8_11);
//  _mm256_storeu_pd(Y_ptr+12 ,Y_12_15);
}

#elif defined (AVX)
//specialized template for double with AVX
template<>
void axpy_kernal(const double a, const double* X_ptr, double* Y_ptr)
{
  //I can see several ways of ordering this...
  _mm_prefetch(Y_ptr+64, _MM_HINT_T2);
  _mm_prefetch(X_ptr+64, _MM_HINT_T2);

  const __m256d a_0_3 = _mm256_broadcast_sd(&a);
  const __m256d X_0_3 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+0);
  const __m256d X_4_7 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+4);

  __m256d Y_0_3 = _mm256_add_pd(_mm256_loadu_pd(Y_ptr+0),X_0_3);
  __m256d Y_4_7 = _mm256_add_pd(_mm256_loadu_pd(Y_ptr+4),X_4_7);
  
  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}
#endif 

//General code
template <typename T>
void libj::axpy(const T a, const libj::tensor<T>& X, libj::tensor<T>& Y)
{
  //check for shape
  if (!same_shape(X,Y)) 
  {
    printf("ERROR libj::axpy tensors X and Y do not have the same shape\n");
    exit(1);
  }

  //check for simple cases
  if (a == (T) 1)
  {
    libj::xpy(X,Y); return;
  } else if (a == (T) 0) {
    return; //do nothing to Y 
  } 

  //General axpy loops
  const long N = X.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    Y[i] = a*X[i] + Y[i]; 
  }

  
  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) shared(X,Y)//if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    axpy_kernal<T>(a,&X[i],&Y[i]);
  }


}
template void libj::axpy<double>(const double a, const libj::tensor<double>& X,
                                 libj::tensor<double>& Y);
template void libj::axpy<float>(const float a, const libj::tensor<float>& X,
                                libj::tensor<float>& Y);
template void libj::axpy<long>(const long a, const libj::tensor<long>& X,
                               libj::tensor<long>& Y);
template void libj::axpy<int>(const int a, const libj::tensor<int>& X,
                              libj::tensor<int>& Y);


