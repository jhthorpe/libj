/*----------------------------------------------------------------------
  sum.cpp 
	JHT, April 13, 2022 : created

  .cpp file for the sum function, which calculates the sum of all 
    elements of a tensor

----------------------------------------------------------------------*/
#include "jblis.hpp"

//Definitions
#define OMP_CHUNK 1024
#if defined (AVX)
  #define UNROLL 16
#else
  #define UNROLL 8
#endif

//AVX instructions
#if defined (AVX) 
template <typename T>
void sum_kernal(const T* A_ptr, T* SUM_ptr)
{
  SUM_ptr[0] += A_ptr[0]  + A_ptr[1];
  SUM_ptr[1] += A_ptr[2]  + A_ptr[3];
  SUM_ptr[2] += A_ptr[4]  + A_ptr[5];
  SUM_ptr[3] += A_ptr[6]  + A_ptr[7];
  SUM_ptr[4] += A_ptr[8]  + A_ptr[9];
  SUM_ptr[5] += A_ptr[10] + A_ptr[11];
  SUM_ptr[6] += A_ptr[12] + A_ptr[13];
  SUM_ptr[7] += A_ptr[14] + A_ptr[15];
}
template void sum_kernal<float>(const float* A_ptr, float* SUM_ptr); 
template void sum_kernal<long>(const long* A_ptr, long* SUM_ptr); 
template void sum_kernal<int>(const int* A_ptr, int* SUM_ptr); 

//Specialized template for double with AVX
template<>
void sum_kernal(const double* A_ptr, double* SUM_ptr)
{
  __m256d S_0_3 = _mm256_loadu_pd(SUM_ptr+0);
  __m256d S_4_7 = _mm256_loadu_pd(SUM_ptr+0);
  S_0_3 += _mm256_hadd_pd(_mm256_loadu_pd(A_ptr+0),_mm256_loadu_pd(A_ptr+4));
  S_4_7 += _mm256_hadd_pd(_mm256_loadu_pd(A_ptr+8),_mm256_loadu_pd(A_ptr+12));
  _mm256_storeu_pd(SUM_ptr+0,S_0_3);
  _mm256_storeu_pd(SUM_ptr+4,S_4_7);

}

//General code without AVX
#else
template <typename T>
void sum_kernal(const T* A_ptr, T* SUM_ptr)
{
  SUM_ptr[0] += A_ptr[0] + A_ptr[1];
  SUM_ptr[1] += A_ptr[2] + A_ptr[3];
  SUM_ptr[2] += A_ptr[4] + A_ptr[5];
  SUM_ptr[3] += A_ptr[6] + A_ptr[7];
}
template void sum_kernal<double>(const double* A_ptr, double* sum_ptr); 
template void sum_kernal<float>(const float* A_ptr, float* sum_ptr); 
template void sum_kernal<long>(const long* A_ptr, long* sum_ptr); 
template void sum_kernal<int>(const int* A_ptr, inst* sum_ptr); 
#endif 

//General code
template <typename T>
T libj::sum(const libj::tensor<T>& A)
{
  const long N = A.size();
  const long M = N % UNROLL;
  int num_threads;
  T SUM;

  //Cleanup loops
  for (long i=0;i<M;i++)
  {
    SUM += A[i]; 
  }

  #pragma omp parallel
  {
    //this might cause false sharing
    T LOCAL_SUM[UNROLL]; 
    for (int i=0;i<UNROLL;i++) {LOCAL_SUM[i] = (T) 0;}

    #pragma omp for schedule(dynamic,OMP_CHUNK) //if (N > OMP_CHUNK)
    for (long i=M;i<N;i+=UNROLL)
    {
      sum_kernal<T>(&A[i],LOCAL_SUM);
    }

    #pragma omp critical 
    { 
      for (long i=0;i<UNROLL;i++)
      {
        SUM += LOCAL_SUM[i];
      } 
    }

  }

  return SUM;

}
template double libj::sum<double>(const libj::tensor<double>& A);
template float libj::sum<float>(const libj::tensor<float>& A);
template long libj::sum<long>(const libj::tensor<long>& A);
template int libj::sum<int>(const libj::tensor<int>& A);


