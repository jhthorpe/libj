/*----------------------------------------------------------------------
  zero.cpp
	JHT, April 11, 2022 : created

  .cpp file for the zero function, which sets a tensor to zero

  The general code is at the bottom, specialized double code at
    the top
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
void zero_kernal(T* A_ptr)
{
  A_ptr[0] = (T) 0;
  A_ptr[1] = (T) 0;
  A_ptr[2] = (T) 0;
  A_ptr[3] = (T) 0;
  A_ptr[4] = (T) 0;
  A_ptr[5] = (T) 0;
  A_ptr[6] = (T) 0;
  A_ptr[7] = (T) 0;
  A_ptr[8] = (T) 0;
  A_ptr[9] = (T) 0;
  A_ptr[10] = (T) 0;
  A_ptr[11] = (T) 0;
  A_ptr[12] = (T) 0;
  A_ptr[13] = (T) 0;
  A_ptr[14] = (T) 0;
  A_ptr[15] = (T) 0;
}
template void zero_kernal<float>(float* A_ptr); 
template void zero_kernal<long>(long* A_ptr); 
template void zero_kernal<int>(int* A_ptr); 

//Specialized template for double
template<>
void zero_kernal(double* A_ptr)
{
  const __m256d z03 = _mm256_setzero_pd();
  _mm256_storeu_pd(A_ptr+0,z03);
  _mm256_storeu_pd(A_ptr+4,z03);
  _mm256_storeu_pd(A_ptr+8,z03);
  _mm256_storeu_pd(A_ptr+12,z03);
}

//General code without AVX
#else
template <typename T>
void zero_kernal(T* A_ptr)
{
  A_ptr[0] = (T) 0;
  A_ptr[1] = (T) 0;
  A_ptr[2] = (T) 0;
  A_ptr[3] = (T) 0;
  A_ptr[4] = (T) 0;
  A_ptr[5] = (T) 0;
}
template void zero_kernal<double>(double* A_ptr); 
template void zero_kernal<float>(float* A_ptr); 
template void zero_kernal<long>(long* A_ptr); 
template void zero_kernal<int>(int* A_ptr); 
#endif 

//General code
template <typename T>
void libj::zero(libj::tensor<T>& A)
{
  const long N = A.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    A[i] = (T) 0;
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    zero_kernal<T>(&A[i]);
  }

}
template void libj::zero<double>(libj::tensor<double>& A);
template void libj::zero<float>(libj::tensor<float>& A);
template void libj::zero<long>(libj::tensor<long>& A);
template void libj::zero<int>(libj::tensor<int>& A);


