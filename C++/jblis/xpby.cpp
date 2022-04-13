/*----------------------------------------------------------------------
  xpby.cpp
	JHT, April 11, 2022 : created

  .cpp file for the xpby function, which performs

  Y = X + b*Y 
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
void xpby_kernal(const T* X_ptr, const T b, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = X_ptr[0] + b*Y_ptr[0]; 
  Y_tmp[1] = X_ptr[1] + b*Y_ptr[1]; 
  Y_tmp[2] = X_ptr[2] + b*Y_ptr[2]; 
  Y_tmp[3] = X_ptr[3] + b*Y_ptr[3]; 
  Y_tmp[4] = X_ptr[4] + b*Y_ptr[4]; 
  Y_tmp[5] = X_ptr[5] + b*Y_ptr[5]; 
  Y_tmp[6] = X_ptr[6] + b*Y_ptr[6]; 
  Y_tmp[7] = X_ptr[7] + b*Y_ptr[7]; 

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
  Y_ptr[4] = Y_tmp[4];
  Y_ptr[5] = Y_tmp[5];
  Y_ptr[6] = Y_tmp[6];
  Y_ptr[7] = Y_tmp[7];
}
template void xpby_kernal<float>(const float* X_ptr, 
                                  const float b, float* Y_ptr); 
template void xpby_kernal<long>(const long* X_ptr,
                                 const long b, long* Y_ptr); 
template void xpby_kernal<int>(const int* X_ptr,
                                const int b, int* Y_ptr); 

//General code without AVX
#else
template <typename T>
void xpby_kernal(const T* X_ptr, const T b, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = X_ptr[0] + b*Y_ptr[0];
  Y_tmp[1] = X_ptr[1] + b*Y_ptr[1];
  Y_tmp[2] = X_ptr[2] + b*Y_ptr[2];
  Y_tmp[3] = X_ptr[3] + b*Y_ptr[3];

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
}
template void xpby_kernal<double>(const double* X_ptr,
                                  const double b, double* Y_ptr); 
template void xpby_kernal<float>(const float* X_ptr,
                                 const float b, float* Y_ptr); 
template void xpby_kernal<long>(const long* X_ptr,
                                const long b, long* Y_ptr); 
template void xpby_kernal<int>(const int* X_ptr,
                               const int b, int* Y_ptr); 
#endif 


//Specialized code for doubles
#if defined (FMA)
template<>
void xpby_kernal(const double* X_ptr, const double b, double* Y_ptr)
{
  const __m256d b_0_3 = _mm256_broadcast_sd(&b);
  __m256d Y_0_3 = _mm256_loadu_pd(Y_ptr+0); 
  __m256d Y_4_7 = _mm256_loadu_pd(Y_ptr+4); 

  Y_0_3 = _mm256_fmadd_pd(b_0_3,Y_0_3,_mm256_loadu_pd(X_ptr+0));
  Y_4_7 = _mm256_fmadd_pd(b_0_3,Y_4_7,_mm256_loadu_pd(X_ptr+4));

  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}

#elif defined (AVX)
//specialized template for double with AVX
template<>
void xpby_kernal(const double* X_ptr, double* Y_ptr)
{
  //I can see several ways of ordering this...

  const __m256d b_0_3 = _mm256_broadcast_sd(&b);
  const __m256d Y_0_3 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+0));
  const __m256d Y_4_7 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+4));

  Y_0_3 = _mm256_add_pd(Y_0_3,_mm256_loadu_pd(X_ptr+0));
  Y_4_7 = _mm256_add_pd(Y_4_7,_mm256_loadu_pd(X_ptr+4));
 
  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}

#endif


//General code around kernal
template <typename T>
void libj::xpby(const libj::tensor<T>& X, const T b, libj::tensor<T>& Y)
{
  //check for shape
  if (!same_shape(X,Y)) 
  {
    printf("ERROR libj::xpby tensors X and Y do not have the same shape\n");
    exit(1);
  }

  //check for simple cases
  if (b == (T) 1)
  {
    libj::xpy(X,Y); return;
  } else if (b == (T) 0) {
    libj::copy(X,Y);
  } 

  //General xpby loops
  const long N = X.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    Y[i] = X[i] + b*Y[i]; 
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    xpby_kernal<T>(&X[i],b,&Y[i]);
  }

}
template void libj::xpby<double>(const libj::tensor<double>& X,
                                 const double b, libj::tensor<double>& Y);
template void libj::xpby<float>(const libj::tensor<float>& X,
                                const float b, libj::tensor<float>& Y);
template void libj::xpby<long>(const libj::tensor<long>& X,
                               const long b, libj::tensor<long>& Y);
template void libj::xpby<int>(const libj::tensor<int>& X,
                              const int b, libj::tensor<int>& Y);


