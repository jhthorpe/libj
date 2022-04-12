/*----------------------------------------------------------------------
  set.cpp
	JHT, April 11, 2022 : created

  .cpp file for the set function, which sets a tensor to a scalar 

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
void set_kernal(const T scal, T* A_ptr)
{
  A_ptr[0] = scal;
  A_ptr[1] = scal;
  A_ptr[2] = scal;
  A_ptr[3] = scal;
  A_ptr[4] = scal;
  A_ptr[5] = scal;
  A_ptr[6] = scal;
  A_ptr[7] = scal;
  A_ptr[8] = scal;
  A_ptr[9] = scal;
  A_ptr[10] = scal;
  A_ptr[11] = scal;
  A_ptr[12] = scal;
  A_ptr[13] = scal;
  A_ptr[14] = scal;
  A_ptr[15] = scal;
}
template void set_kernal<float>(const float scal, float* A_ptr); 
template void set_kernal<long>(const long scal, long* A_ptr); 
template void set_kernal<int>(const int scal, int* A_ptr); 

//Specialized template for double with AVX
template<>
void set_kernal(const double scal, double* A_ptr)
{
  const __m256d a03 = _mm256_broadcast_sd(&scal);
  _mm256_storeu_pd(A_ptr+0,a03);
  _mm256_storeu_pd(A_ptr+4,a03);
  _mm256_storeu_pd(A_ptr+8,a03);
  _mm256_storeu_pd(A_ptr+12,a03);
}

//General code without AVX
#else
template <typename T>
void set_kernal(const T scal, T* A_ptr)
{
  A_ptr[0] = scal; 
  A_ptr[1] = scal; 
  A_ptr[2] = scal; 
  A_ptr[3] = scal; 
  A_ptr[4] = scal; 
  A_ptr[5] = scal; 
}
template void set_kernal<double>(const double scal, double* A_ptr); 
template void set_kernal<float>(const float scal, float* A_ptr); 
template void set_kernal<long>(const long scal, long* A_ptr); 
template void set_kernal<int>(const int scal, int* A_ptr); 
#endif 

//General code
template <typename T>
void libj::set(const T scal, libj::tensor<T>& A)
{
  const long N = A.size();
  const long M = N % UNROLL;

  for (long i=0;i<M;i++)
  {
    A[i] = scal;
  }

  #pragma omp parallel for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
  for (long i=M;i<N;i+=UNROLL)
  {
    set_kernal<T>(scal,&A[i]);
  }

}
template void libj::set<double>(const double scal, libj::tensor<double>& A);
template void libj::set<float>(const float scal, libj::tensor<float>& A);
template void libj::set<long>(const long scal, libj::tensor<long>& A);
template void libj::set<int>(const int scal, libj::tensor<int>& A);


