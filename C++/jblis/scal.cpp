/*----------------------------------------------------------------------
  scal.cpp
	JHT, April 11, 2022 : created

  .cpp file for the scal function, which scals a tensor to a scalar 

----------------------------------------------------------------------*/
#include "jblis.hpp"

//Definitions
#define OMP_CHUNK 1024
#if defined (AVX)
  #define UNROLL 16
#else
  #define UNROLL 6
#endif

//AVX instructions
#if defined (AVX) 
template <typename T>
void scal_kernal(const T s, T* A_ptr)
{
  A_ptr[0] *= s;
  A_ptr[1] *= s;
  A_ptr[2] *= s;
  A_ptr[3] *= s;
  A_ptr[4] *= s;
  A_ptr[5] *= s;
  A_ptr[6] *= s;
  A_ptr[7] *= s;
  A_ptr[8] *= s;
  A_ptr[9] *= s;
  A_ptr[10] *= s;
  A_ptr[11] *= s;
  A_ptr[12] *= s;
  A_ptr[13] *= s;
  A_ptr[14] *= s;
  A_ptr[15] *= s;
}
template void scal_kernal<float>(const float s, float* A_ptr); 
template void scal_kernal<long>(const long s, long* A_ptr); 
template void scal_kernal<int>(const int s, int* A_ptr); 

//Specialized template for double with AVX
template<>
void scal_kernal(const double s, double* A_ptr)
{
  const __m256d a_0_3 = _mm256_broadcast_sd(&s);
  __m256d A_0_3   = _mm256_loadu_pd(A_ptr+0);
  __m256d A_4_7   = _mm256_loadu_pd(A_ptr+4);
  __m256d A_8_11  = _mm256_loadu_pd(A_ptr+8);
  __m256d A_12_15 = _mm256_loadu_pd(A_ptr+12);

  A_0_3   = _mm256_mul_pd(A_0_3,a_0_3);
  A_4_7   = _mm256_mul_pd(A_4_7,a_0_3);
  A_8_11  = _mm256_mul_pd(A_8_11,a_0_3);
  A_12_15 = _mm256_mul_pd(A_12_15,a_0_3);

  _mm256_storeu_pd(A_ptr+0 ,A_0_3);
  _mm256_storeu_pd(A_ptr+4 ,A_4_7);
  _mm256_storeu_pd(A_ptr+8 ,A_8_11);
  _mm256_storeu_pd(A_ptr+12,A_12_15);
}

//General code without AVX
#else
template <typename T>
void scal_kernal(const T s, T* A_ptr)
{
  A_ptr[0] *= s; 
  A_ptr[1] *= s; 
  A_ptr[2] *= s; 
  A_ptr[3] *= s; 
  A_ptr[4] *= s; 
  A_ptr[5] *= s; 
}
template void scal_kernal<double>(const double s, double* A_ptr); 
template void scal_kernal<float>(const float s, float* A_ptr); 
template void scal_kernal<long>(const long s, long* A_ptr); 
template void scal_kernal<int>(const int s, int* A_ptr); 
#endif 

//General code
template <typename T>
void libj::scal(const T s, libj::tensor<T>& A)
{
  //check for simple cases
  if (s == (T) 0)
  {
    zero(A); return;
  } else if (s == (T) 1) {
    return;
  }

  const long N = A.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    A[i] *= s;
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    scal_kernal<T>(s,&A[i]);
  }

}
template void libj::scal<double>(const double s, libj::tensor<double>& A);
template void libj::scal<float>(const float s, libj::tensor<float>& A);
template void libj::scal<long>(const long s, libj::tensor<long>& A);
template void libj::scal<int>(const int s, libj::tensor<int>& A);


