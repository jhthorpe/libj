/* simd_elemwise_mul.cpp
 * JHT, November 15, 2021 : created
 *
 * .cpp file that implements simd elementwise multiplication operations
 * between two continuous sections of data, stored in a third.
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * Element wise multiplication without (known) alignment
 *
 *---------------------------------------------------------------------*/
template <typename T>
void simd_elemwise_mul(const long N, const T* X, const T* Y, T* Z)
{
  #if defined (_OPENMP)
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      *(Z+i) = *(X+i) * *(Y+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(Z+i+0) = *(X+i+0) * *(Y+i+0);
      *(Z+i+1) = *(X+i+1) * *(Y+i+1);
      *(Z+i+2) = *(X+i+2) * *(Y+i+2);
      *(Z+i+3) = *(X+i+3) * *(Y+i+3);
    }
    
    for (i=i;i<N;i++)
    {
      *(Z+i) = *(X+i) * *(Y+i);
    }
  #endif
}
template void simd_elemwise_mul<double>(const long N, const double* X, const double* Y, double* Z);
template void simd_elemwise_mul<float>(const long N, const float* X, const float* Y, float* Z);
template void simd_elemwise_mul<long>(const long N, const long* X, const long* Y, long* Z);
template void simd_elemwise_mul<int>(const long N, const int* X, const int* Y, int* Z);


/*---------------------------------------------------------------------
 * Element wise multiplication with known alignment
 *
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_elemwise_mul(const long N, const T* X, const T* Y, T* Z)
{
  #if defined (_OPENMP)
    #pragma omp simd  aligned(X,Y,Z:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      *(Z+i) = *(X+i) * *(Y+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(Z+i+0) = *(X+i+0) * *(Y+i+0);
      *(Z+i+1) = *(X+i+1) * *(Y+i+1);
      *(Z+i+2) = *(X+i+2) * *(Y+i+2);
      *(Z+i+3) = *(X+i+3) * *(Y+i+3);
    }
    
    for (i=i;i<N;i++)
    {
      *(Z+i) = *(X+i) * *(Y+i);
    }
  #endif
}

template void simd_elemwise_mul<double,64>(const long N, const double* X, const double* Y, double* Z);
template void simd_elemwise_mul<double,32>(const long N, const double* X, const double* Y, double* Z);
template void simd_elemwise_mul<double,16>(const long N, const double* X, const double* Y, double* Z);
template void simd_elemwise_mul<double,8>(const long N, const double* X, const double* Y, double* Z);

template void simd_elemwise_mul<float,64>(const long N, const float* X, const float* Y, float* Z);
template void simd_elemwise_mul<float,32>(const long N, const float* X, const float* Y, float* Z);
template void simd_elemwise_mul<float,16>(const long N, const float* X, const float* Y, float* Z);
template void simd_elemwise_mul<float,8>(const long N, const float* X, const float* Y, float* Z);
template void simd_elemwise_mul<float,4>(const long N, const float* X, const float* Y, float* Z);

template void simd_elemwise_mul<long,64>(const long N, const long* X, const long* Y, long* Z);
template void simd_elemwise_mul<long,32>(const long N, const long* X, const long* Y, long* Z);
template void simd_elemwise_mul<long,16>(const long N, const long* X, const long* Y, long* Z);
template void simd_elemwise_mul<long,8>(const long N, const long* X, const long* Y, long* Z);

template void simd_elemwise_mul<int,64>(const long N, const int* X, const int* Y, int* Z);
template void simd_elemwise_mul<int,32>(const long N, const int* X, const int* Y, int* Z);
template void simd_elemwise_mul<int,16>(const long N, const int* X, const int* Y, int* Z);
template void simd_elemwise_mul<int,8>(const long N, const int* X, const int* Y, int* Z);
template void simd_elemwise_mul<int,4>(const long N, const int* X, const int* Y, int* Z);
