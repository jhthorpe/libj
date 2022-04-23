/*----------------------------------------------------------------------
  awxpby.cpp
	JHT, April 21, 2022 : created

  .cpp file for the awxpby function, which performs

  Y = a*W*X + b*Y 

  Where W*X is an elementwise multiplication
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
void awxpby_kernal(const T a, const T* W_ptr, const T* X_ptr, const T b, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*W_ptr[0]*X_ptr[0] + b*Y_ptr[0]; 
  Y_tmp[1] = a*W_ptr[1]*X_ptr[1] + b*Y_ptr[1]; 
  Y_tmp[2] = a*W_ptr[2]*X_ptr[2] + b*Y_ptr[2]; 
  Y_tmp[3] = a*W_ptr[3]*X_ptr[3] + b*Y_ptr[3]; 
  Y_tmp[4] = a*W_ptr[4]*X_ptr[4] + b*Y_ptr[4]; 
  Y_tmp[5] = a*W_ptr[5]*X_ptr[5] + b*Y_ptr[5]; 
  Y_tmp[6] = a*W_ptr[6]*X_ptr[6] + b*Y_ptr[6]; 
  Y_tmp[7] = a*W_ptr[7]*X_ptr[7] + b*Y_ptr[7]; 

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
  Y_ptr[4] = Y_tmp[4];
  Y_ptr[5] = Y_tmp[5];
  Y_ptr[6] = Y_tmp[6];
  Y_ptr[7] = Y_tmp[7];
}
template void awxpby_kernal<float>(const float a, const float* W_ptr, const float* X_ptr, 
                                   const float b, float* Y_ptr); 
template void awxpby_kernal<long>(const long a, const long* W_ptr, const long* X_ptr,
                                  const long b, long* Y_ptr); 
template void awxpby_kernal<int>(const int a, const int* W_ptr, const int* X_ptr,
                                 const int b, int* Y_ptr); 

//General code without AVX or FMA
#else
template <typename T>
void awxpby_kernal(const T a, const T* W_ptr, const T* X_ptr, const T b, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*W_ptr[0]*X_ptr[0] + b*Y_ptr[0];
  Y_tmp[1] = a*W_ptr[1]*X_ptr[1] + b*Y_ptr[1];
  Y_tmp[2] = a*W_ptr[2]*X_ptr[2] + b*Y_ptr[2];
  Y_tmp[3] = a*W_ptr[3]*X_ptr[3] + b*Y_ptr[3];

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
}
template void awxpby_kernal<double>(const double a, const double* W_ptr, const double* X_ptr,
                                   const double b, double* Y_ptr); 
template void awxpby_kernal<float>(const float* a, const float* W_ptr, const float* X_ptr,
                                  const float b, float* Y_ptr); 
template void awxpby_kernal<long>(const long a, const long* W_ptr, const long* X_ptr,
                                 const long b, long* Y_ptr); 
template void awxpby_kernal<int>(const int a, const int* W_ptr, const int* X_ptr,
                                const int b, int* Y_ptr); 
#endif 


//Specialized code for doubles
#if defined (FMA)
template<>
inline void awxpby_kernal(const double a, const double* W_ptr, const double* X_ptr, 
                   const double b, double* Y_ptr)
{
  _mm_prefetch(Y_ptr+0, _MM_HINT_T0);
  _mm_prefetch(X_ptr+0, _MM_HINT_T0);
  _mm_prefetch(W_ptr+0, _MM_HINT_T0);

  const __m256d a_0_3 = _mm256_broadcast_sd(&a);
  const __m256d b_0_3 = _mm256_broadcast_sd(&b);

  const __m256d X_0_3 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+0));
  const __m256d X_4_7 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+4));

  __m256d Y_0_3 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+0)); 
  __m256d Y_4_7 = _mm256_mul_pd(b_0_3,_mm256_loadu_pd(Y_ptr+4)); 

  Y_0_3 = _mm256_fmadd_pd(X_0_3,_mm256_loadu_pd(W_ptr+0),Y_0_3);
  Y_4_7 = _mm256_fmadd_pd(X_4_7,_mm256_loadu_pd(W_ptr+4),Y_4_7);

  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);

/*
  const __m256d a_0_3 = _mm256_broadcast_sd(&a);
  const __m256d b_0_3 = _mm256_broadcast_sd(&b);

  const __m256d X_0_3 = _mm256_mul_pd(a_0_3,_mm256_load_pd(X_ptr+0));
  const __m256d X_4_7 = _mm256_mul_pd(a_0_3,_mm256_load_pd(X_ptr+4));

  __m256d Y_0_3 = _mm256_mul_pd(b_0_3,_mm256_load_pd(Y_ptr+0)); 
  __m256d Y_4_7 = _mm256_mul_pd(b_0_3,_mm256_load_pd(Y_ptr+4)); 

  Y_0_3 = _mm256_fmadd_pd(X_0_3,_mm256_load_pd(W_ptr+0),Y_0_3);
  Y_4_7 = _mm256_fmadd_pd(X_4_7,_mm256_load_pd(W_ptr+4),Y_4_7);

//  _mm256_store_pd(Y_ptr+0 ,Y_0_3);
//  _mm256_store_pd(Y_ptr+4 ,Y_4_7);
  _mm256_stream_pd(Y_ptr+0 ,Y_0_3);
  _mm256_stream_pd(Y_ptr+4 ,Y_4_7);
*/

}

#elif defined (AVX)
//specialized template for double with AVX
template<>
void awxpby_kernal(const double a, const double* W_ptr, const double* X_ptr, 
                   const double b, double* Y_ptr)
{
  _mm_prefetch(Y_ptr+0, _MM_HINT_T0);
  _mm_prefetch(X_ptr+0, _MM_HINT_T0);
  _mm_prefetch(W_ptr+0, _MM_HINT_T0);

  const __m256d a_0_3 = _mm256_broadcast_sd(&a);
  const __m256d b_0_3 = _mm256_broadcast_sd(&b);
  const __m256d X_0_3 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+0));
  const __m256d X_4_7 = _mm256_mul_pd(a_0_3,_mm256_loadu_pd(X_ptr+4));

  X_0_3 = _mm256_mul_pd(X_0_3,_mm256_loadu_pd(W_ptr+0));
  X_4_7 = _mm256_mul_pd(X_4_7,_mm256_loadu_pd(W_ptr+4));

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
void libj::awxpby(const T a, const libj::tensor<T>& W, const libj::tensor<T>& X, 
                  const T b, libj::tensor<T>& Y)
{
  //check for shape
  if (!same_shape(X,Y) || !same_shape(X,W)) 
  {
    printf("ERROR libj::awxpby tensors W,X,Y do not have the same shape\n");
    exit(1);
  }

  //check for simple cases
  //Simple cases currently coded
  // scal  if a==0 ,b!=0
  // zero  if a==0 ,b==0
  // nwxpy if a==-1,b==1  
  if (a == (T) 0 && b != (T) 0) {
    libj::scal(b,Y);
  } else if (a == (T) 0 && b == (T) 0) {
    libj::zero(Y);
  } 

  //General awxpby loops
  const size_t N = X.size();
  const size_t M = N % UNROLL;

  for (size_t i=0;i<M;i++)
  {
    Y[i] = a*W[i]*X[i] + b*Y[i]; 
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (size_t i=M;i<N;i+=UNROLL)
  {
    awxpby_kernal<T>(a,&W[i],&X[i],b,&Y[i]);
  }

}
template void libj::awxpby<double>(const double a, const libj::tensor<double>& W,
                                   const libj::tensor<double>& X,
                                   const double b, libj::tensor<double>& Y);
template void libj::awxpby<float>(const float a, const libj::tensor<float>& W,
                                  const libj::tensor<float>& X,
                                  const float b, libj::tensor<float>& Y);
template void libj::awxpby<long>(const long a, const libj::tensor<long>& W,
                                 const libj::tensor<long>& X,
                                 const long b, libj::tensor<long>& Y);
template void libj::awxpby<int>(const int a, const libj::tensor<int>& W,
                                const libj::tensor<int>& X,
                                const int b, libj::tensor<int>& Y);

