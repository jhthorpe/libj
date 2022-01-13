/* simd_dot.cpp
 * JHT, December 8, 2021 : created 
 *
 * .cpp file that implements simd dot-product operation 
 * between two continuous sections of data
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * dot without (known) alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *---------------------------------------------------------------------*/
template <typename T>
T simd_dot(const long N, const T* X, const T* Y)
{
  T dot = 0;
  #if defined (_OPENMP)
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      dot += *(X+i) * *(Y+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      dot += *(X+i+0) * *(Y+i+0);
      dot += *(X+i+1) * *(Y+i+1);
      dot += *(X+i+2) * *(Y+i+2);
      dot += *(X+i+3) * *(Y+i+3);
    }
    
    for (i=i;i<N;i++)
    {
      dot += *(X+i) * *(Y+i);
    }
  #endif
  return dot;
}
template double simd_dot<double>(const long N, const double* X, const double* Y);
template float simd_dot<float>(const long N, const float* X, const float* Y);
template long simd_dot<long>(const long N, const long* X, const long* Y);
template int simd_dot<int>(const long N, const int* X, const int* Y);


/*---------------------------------------------------------------------
 * Element wise addition with known alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *
 * Future versions will use specifically vectorized code
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
T simd_dot(const long N, const T* X, const T* Y)
{
  T dot = 0;
  #if defined (_OPENMP)
    #pragma omp simd  aligned(X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      dot += *(X+i) * *(Y+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      dot += *(X+i+0) * *(Y+i+0);
      dot += *(X+i+1) * *(Y+i+1);
      dot += *(X+i+2) * *(Y+i+2);
      dot += *(X+i+3) * *(Y+i+3);
    }
    
    for (i=i;i<N;i++)
    {
      dot += *(X+i) * *(Y+i);
    }
  #endif
  return dot;
}

template double simd_dot<double,128>(const long N, const double* X, const double* Y);
template double simd_dot<double,64>(const long N, const double* X, const double* Y);
template double simd_dot<double,32>(const long N, const double* X, const double* Y);
template double simd_dot<double,16>(const long N, const double* X, const double* Y);
template double simd_dot<double,8>(const long N, const double* X, const double* Y);

template float simd_dot<float,128>(const long N, const float* X, const float* Y);
template float simd_dot<float,64>(const long N, const float* X, const float* Y);
template float simd_dot<float,32>(const long N, const float* X, const float* Y);
template float simd_dot<float,16>(const long N, const float* X, const float* Y);
template float simd_dot<float,8>(const long N, const float* X, const float* Y);
template float simd_dot<float,4>(const long N, const float* X, const float* Y);

template long simd_dot<long,128>(const long N, const long* X, const long* Y);
template long simd_dot<long,64>(const long N, const long* X, const long* Y);
template long simd_dot<long,32>(const long N, const long* X, const long* Y);
template long simd_dot<long,16>(const long N, const long* X, const long* Y);
template long simd_dot<long,8>(const long N, const long* X, const long* Y);

template int simd_dot<int,128>(const long N, const int* X, const int* Y);
template int simd_dot<int,64>(const long N, const int* X, const int* Y);
template int simd_dot<int,32>(const long N, const int* X, const int* Y);
template int simd_dot<int,16>(const long N, const int* X, const int* Y);
template int simd_dot<int,8>(const long N, const int* X, const int* Y);
template int simd_dot<int,4>(const long N, const int* X, const int* Y);
