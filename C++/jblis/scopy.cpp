/*----------------------------------------------------------------------
  scopy.cpp
	JHT, April 11, 2022 : created

  .cpp file for the scopy function, which copies a scaled tensor X
  into tensor Y of the same shape.

----------------------------------------------------------------------*/
#include "jblis.hpp"

//Definitions
#define OMP_CHUNK 1024
#if defined (AVX)
  #define UNROLL 8
#else
  #define UNROLL 4
#endif

//AVX instructions
#if defined (AVX) 
template <typename T>
void scopy_kernal(const T a, const T* X_ptr, T* Y_ptr )
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*X_ptr[0]; 
  Y_tmp[1] = a*X_ptr[1]; 
  Y_tmp[2] = a*X_ptr[2]; 
  Y_tmp[3] = a*X_ptr[3]; 
  Y_tmp[4] = a*X_ptr[4]; 
  Y_tmp[5] = a*X_ptr[5]; 
  Y_tmp[6] = a*X_ptr[6]; 
  Y_tmp[7] = a*X_ptr[7]; 

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
  Y_ptr[4] = Y_tmp[4];
  Y_ptr[5] = Y_tmp[5];
  Y_ptr[6] = Y_tmp[6];
  Y_ptr[7] = Y_tmp[7];
}
template void scopy_kernal<float>(const float a, const float* X_ptr, 
                                  float* Y_ptr); 
template void scopy_kernal<long>(const long a, const long* X_ptr,
                                 long* Y_ptr); 
template void scopy_kernal<int>(const int a, const int* X_ptr,
                                int* Y_ptr); 

//Specialized template for double with AVX
template<>
void scopy_kernal(const double a, const double* X_ptr, double* Y_ptr)
{
  const __m256d a_0_3 = _mm256_broadcast_sd(&a);

  const __m256d X_0_3 = _mm256_loadu_pd(X_ptr+0);
  const __m256d X_4_7 = _mm256_loadu_pd(X_ptr+4);

  const __m256d Y_0_3 = _mm256_mul_pd(X_0_3,a_0_3);
  const __m256d Y_4_7 = _mm256_mul_pd(X_4_7,a_0_3);

  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}

//General code without AVX
#else
template <typename T>
void scopy_kernal(const T a, const T* X_ptr, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = a*X_ptr[0];
  Y_tmp[1] = a*X_ptr[1];
  Y_tmp[2] = a*X_ptr[2];
  Y_tmp[3] = a*X_ptr[3];

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
}
template void scopy_kernal<double>(const double a, const double* X_ptr,
                                   double* Y_ptr); 
template void scopy_kernal<float>(const float a, const float* X_ptr,
                                  float* Y_ptr); 
template void scopy_kernal<long>(const long a, const long* X_ptr,
                                 long* Y_ptr); 
template void scopy_kernal<int>(const int a, const int* X_ptr,
                                int* Y_ptr); 
#endif 

//General code
template <typename T>
void libj::scopy(const T a, const libj::tensor<T>& X, libj::tensor<T>& Y)
{
  //check for shape
  if (!same_shape(X,Y)) 
  {
    printf("ERROR libj::scopy tensors X and Y do not have the same shape\n");
    exit(1);
  }

  //check for simple cases
  if (a == (T) 1)
  {
    libj::copy<T>(X,Y);
  } else if (a == (T) 0) {
    libj::zero<T>(Y);
  }

  //General scopy loops
  const long N = X.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    Y[i] = a*X[i]; 
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    scopy_kernal<T>(a,&X[i],&Y[i]);
  }

}
template void libj::scopy<double>(const double a, const libj::tensor<double>& X,
                                  libj::tensor<double>& Y);
template void libj::scopy<float>(const float a, const libj::tensor<float>& X,
                                  libj::tensor<float>& Y);
template void libj::scopy<long>(const long a, const libj::tensor<long>& X,
                                libj::tensor<long>& Y);
template void libj::scopy<int>(const int a, const libj::tensor<int>& X,
                               libj::tensor<int>& Y);


