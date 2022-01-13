/* simd_scal_mul.cpp
 * JHT, December 8, 2021 : created 
 *
 * .cpp file that implements simd scalar multiplication
 * of a vector of contiguous memory 
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * scal without (known) alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *---------------------------------------------------------------------*/
template <typename T>
void simd_scal_mul(const long N, const T A, T* X)
{
  #if defined (_OPENMP)
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      *(X+i) *= A; 
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(X+i+0) *= A;
      *(X+i+1) *= A;
      *(X+i+2) *= A;
      *(X+i+3) *= A;
    }
    
    for (i=i;i<N;i++)
    {
      *(X+i) *= A; 
    }
  #endif
}
template void simd_scal_mul<double>(const long N, const double A, double* X);
template void simd_scal_mul<float>(const long N, const float A, float* X);
template void simd_scal_mul<long>(const long N, const long A, long* X);
template void simd_scal_mul<int>(const long N, const int A, int* X);


/*---------------------------------------------------------------------
 * scal with known alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *
 * Future versions will use specifically vectorized code
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_scal_mul(const long N, const T A, T* X)
{
  #if defined (_OPENMP)
    #pragma omp simd  aligned(X:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      *(X+i) *= A; 
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(X+i+0) *= A;
      *(X+i+1) *= A;
      *(X+i+2) *= A;
      *(X+i+3) *= A;
    }
    
    for (i=i;i<N;i++)
    {
      *(X+i) *= A;
    }
  #endif
}

template void simd_scal_mul<double,128>(const long N, const double A, double* X);
template void simd_scal_mul<double,64>(const long N, const double A, double* X);
template void simd_scal_mul<double,32>(const long N, const double A, double* X);
template void simd_scal_mul<double,16>(const long N, const double A, double* X);
template void simd_scal_mul<double,8>(const long N, const double A, double* X);

template void simd_scal_mul<float,128>(const long N, const float A, float* X);
template void simd_scal_mul<float,64>(const long N, const float A, float* X);
template void simd_scal_mul<float,32>(const long N, const float A, float* X);
template void simd_scal_mul<float,16>(const long N, const float A, float* X);
template void simd_scal_mul<float,8>(const long N, const float A, float* X);
template void simd_scal_mul<float,4>(const long N, const float A, float* X);

template void simd_scal_mul<long,128>(const long N, const long A, long* X);
template void simd_scal_mul<long,64>(const long N, const long A, long* X);
template void simd_scal_mul<long,32>(const long N, const long A, long* X);
template void simd_scal_mul<long,16>(const long N, const long A, long* X);
template void simd_scal_mul<long,8>(const long N, const long A, long* X);

template void simd_scal_mul<int,128>(const long N, const int A, int* X);
template void simd_scal_mul<int,64>(const long N, const int A, int* X);
template void simd_scal_mul<int,32>(const long N, const int A, int* X);
template void simd_scal_mul<int,16>(const long N, const int A, int* X);
template void simd_scal_mul<int,8>(const long N, const int A, int* X);
template void simd_scal_mul<int,4>(const long N, const int A, int* X);
