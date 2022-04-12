/*----------------------------------------------------------------------
  xpy.cpp
	JHT, April 11, 2022 : created

  .cpp file for the xpy function, which performs

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
#if defined (AVX) 
template <typename T>
void xpy_kernal(const T* X_ptr, T* Y_ptr)
{ 
  T Y_tmp[UNROLL] = {}; 
  Y_tmp[0] = X_ptr[0] + Y_ptr[0];
  Y_tmp[1] = X_ptr[1] + Y_ptr[1];
  Y_tmp[2] = X_ptr[2] + Y_ptr[2];
  Y_tmp[3] = X_ptr[3] + Y_ptr[3];
  Y_tmp[4] = X_ptr[4] + Y_ptr[4];
  Y_tmp[5] = X_ptr[5] + Y_ptr[5];
  Y_tmp[6] = X_ptr[6] + Y_ptr[6];
  Y_tmp[7] = X_ptr[7] + Y_ptr[7];
  
  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
  Y_ptr[4] = Y_tmp[4];
  Y_ptr[5] = Y_tmp[5];
  Y_ptr[6] = Y_tmp[6];
  Y_ptr[7] = Y_tmp[7];
}
template void xpy_kernal<float>(const float* X_ptr,
                                float* Y_ptr);
template void xpy_kernal<long>(const long* X_ptr,
                               long* Y_ptr);
template void xpy_kernal<int>(const int* X_ptr,
                              int* Y_ptr);

#else
template <typename T>
void xpy_kernal(const T* X_ptr, T* Y_ptr)
{
  T Y_tmp[UNROLL] = {};
  Y_tmp[0] = X_ptr[0] + Y_ptr[0];
  Y_tmp[1] = X_ptr[1] + Y_ptr[1];
  Y_tmp[2] = X_ptr[2] + Y_ptr[2];
  Y_tmp[3] = X_ptr[3] + Y_ptr[3];

  Y_ptr[0] = Y_tmp[0];
  Y_ptr[1] = Y_tmp[1];
  Y_ptr[2] = Y_tmp[2];
  Y_ptr[3] = Y_tmp[3];
}
template void xpy_kernal<double>(const double* X_ptr,
                                 double* Y_ptr); 
template void xpy_kernal<float>(const float* X_ptr,
                                float* Y_ptr); 
template void xpy_kernal<long>(const long* X_ptr,
                               long* Y_ptr); 
template void xpy_kernal<int>(const int* X_ptr,
                              int* Y_ptr); 
#endif

#if defined (AVX)
//specialized template for double with AVX
template<>
void xpy_kernal(const double* X_ptr, double* Y_ptr)
{
  //I can see several ways of ordering this...

  const __m256d Y_0_3 = _mm256_add_pd(_mm256_loadu_pd(Y_ptr+0),
                                      _mm256_loadu_pd(X_ptr+0));
  const __m256d Y_4_7 = _mm256_add_pd(_mm256_loadu_pd(Y_ptr+4),
                                      _mm256_loadu_pd(X_ptr+4));
  
  _mm256_storeu_pd(Y_ptr+0 ,Y_0_3);
  _mm256_storeu_pd(Y_ptr+4 ,Y_4_7);
}
#endif 

//General code
template <typename T>
void libj::xpy(const libj::tensor<T>& X, libj::tensor<T>& Y)
{
  //check for shape
  if (!same_shape(X,Y)) 
  {
    printf("ERROR libj::xpy tensors X and Y do not have the same shape\n");
    exit(1);
  }

  //General xpy loops
  const long N = X.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    Y[i] = X[i] + Y[i]; 
  }

  
  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    xpy_kernal<T>(&X[i],&Y[i]);
  }

}
template void libj::xpy<double>(const libj::tensor<double>& X,
                                 libj::tensor<double>& Y);
template void libj::xpy<float>(const libj::tensor<float>& X,
                                libj::tensor<float>& Y);
template void libj::xpy<long>(const libj::tensor<long>& X,
                               libj::tensor<long>& Y);
template void libj::xpy<int>(const libj::tensor<int>& X,
                              libj::tensor<int>& Y);


