/* simd_zero.cpp
 * JHT, December 10, 2021 : created 
 *
 * .cpp file that implements a simd zero-ing of a sequential
 * portion of memory 
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * zero without (known) alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *---------------------------------------------------------------------*/
template <typename T>
void simd_zero(const long N, T* X)
{
  #if defined (_OPENMP)
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      *(X+i) = (T) 0; 
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(X+i+0) = (T) 0;
      *(X+i+1) = (T) 0;
      *(X+i+2) = (T) 0;
      *(X+i+3) = (T) 0;
    }
    
    for (i=i;i<N;i++)
    {
      *(X+i) = (T) 0; 
    }
  #endif
}
template void simd_zero<double>(const long N, double* X);
template void simd_zero<float>(const long N, float* X);
template void simd_zero<long>(const long N, long* X);
template void simd_zero<int>(const long N, int* X);


/*---------------------------------------------------------------------
 * zero with known alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *
 * Future versions will use specifically vectorized code
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_zero(const long N, T* X)
{
  #if defined (_OPENMP)
    #pragma omp simd  aligned(X:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      *(X+i) = (T) 0; 
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(X+i+0) = (T) 0;
      *(X+i+1) = (T) 0;
      *(X+i+2) = (T) 0;
      *(X+i+3) = (T) 0;
    }
    
    for (i=i;i<N;i++)
    {
      *(X+i) = (T) 0;
    }
  #endif
}

template void simd_zero<double,128>(const long N, double* X);
template void simd_zero<double,64>(const long N, double* X);
template void simd_zero<double,32>(const long N, double* X);
template void simd_zero<double,16>(const long N, double* X);
template void simd_zero<double,8>(const long N, double* X);

template void simd_zero<float,128>(const long N, float* X);
template void simd_zero<float,64>(const long N, float* X);
template void simd_zero<float,32>(const long N, float* X);
template void simd_zero<float,16>(const long N, float* X);
template void simd_zero<float,8>(const long N, float* X);
template void simd_zero<float,4>(const long N, float* X);

template void simd_zero<long,128>(const long N, long* X);
template void simd_zero<long,64>(const long N, long* X);
template void simd_zero<long,32>(const long N, long* X);
template void simd_zero<long,16>(const long N, long* X);
template void simd_zero<long,8>(const long N, long* X);

template void simd_zero<int,128>(const long N, int* X);
template void simd_zero<int,64>(const long N, int* X);
template void simd_zero<int,32>(const long N, int* X);
template void simd_zero<int,16>(const long N, int* X);
template void simd_zero<int,8>(const long N, int* X);
template void simd_zero<int,4>(const long N, int* X);
