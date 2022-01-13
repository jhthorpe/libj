/* simd_reduction_sub.cpp
 * JHT, October 27, 2021 : created
 *
 * .cpp file that implements simd reduction with - operator
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 * Special, hand-coded routine is used if AVX2 instructions 
 * are available in the case of an Xay of doubles claimed to 
 * be aligned to 32 or 64 BYTE boundaries 
 *
 */


#include "simd.hpp"

//For unaligned templates
template <typename T>
T simd_reduction_sub(const long N, const T* X)
{

  //OpenMP Code
  #if defined (_OPENMP)
    T sum=0;
    #pragma omp simd reduction(-:sum) 
    for (long i=0;i<N;i++)
    {
      sum -= *(X+i); 
    }
    return sum;

  //unrolled loop code
  #else
    T local_sub[5];
    local_sub[0] = (T) 0;
    local_sub[1] = (T) 0;
    local_sub[2] = (T) 0;
    local_sub[3] = (T) 0;
    local_sub[4] = (T) 0;

    long i;
    for (i=0;i<(N-5);i+=5)  
    {
      local_sub[0] -= *(X+i+0);  
      local_sub[1] -= *(X+i+1);  
      local_sub[2] -= *(X+i+2);  
      local_sub[3] -= *(X+i+3);  
      local_sub[4] -= *(X+i+4);  
    }
  
    //cleanup loop
    for (i=i;i<N;i++)
    {
      local_sub[0] -= *(X+i);
    }

    return (local_sub[0] + local_sub[1] + local_sub[2] + local_sub[3] + local_sub[4]);
  #endif
}

//Aligned template
template <typename T, const int ALIGNMENT>
T simd_reduction_sub(const long N, const T* X)
{
  #if defined (_OPENMP)
    T sum=0;
    #pragma omp simd aligned(X:ALIGNMENT) reduction(-:sum) 
    for (long i=0;i<N;i++)
    {
      sum -= *(X+i); 
    }
    return sum;

  #else
    T local_sub[5];
    local_sub[0] = (T) 0;
    local_sub[1] = (T) 0;
    local_sub[2] = (T) 0;
    local_sub[3] = (T) 0;
    local_sub[4] = (T) 0;

    long i;
    for (i=0;i<(N-5);i+=5)  
    {
      local_sub[0] -= *(X+i+0);  
      local_sub[1] -= *(X+i+1);  
      local_sub[2] -= *(X+i+2);  
      local_sub[3] -= *(X+i+3);  
      local_sub[4] -= *(X+i+4);  
    }
  
    //cleanup loop
    for (i=i;i<N;i++)
    {
      local_sub[0] -= *(X+i);
    }

    return (local_sub[0] + local_sub[1] + local_sub[2] + local_sub[3] + local_sub[4]);

  #endif
}

//Template definitions for unaligned simd
template double simd_reduction_sub<double>(const long N, const double* X);
template float simd_reduction_sub<float>(const long N, const float* X);
template long simd_reduction_sub<long>(const long N, const long* X);
template int simd_reduction_sub<int>(const long N, const int* X);

//template defintion for aligned simd 
template double simd_reduction_sub<double,8>(const long N, const double* X);
template double simd_reduction_sub<double,16>(const long N, const double* X);
#if !defined (__AVX2__)
template double simd_reduction_sub<double,32>(const long N, const double* X);
template double simd_reduction_sub<double,64>(const long N, const double* X);
#endif

template float simd_reduction_sub<float,4>(const long N, const float* X);
template float simd_reduction_sub<float,8>(const long N, const float* X);
template float simd_reduction_sub<float,16>(const long N, const float* X);
template float simd_reduction_sub<float,32>(const long N, const float* X);
template float simd_reduction_sub<float,64>(const long N, const float* X);

template long simd_reduction_sub<long,8>(const long N, const long* X);
template long simd_reduction_sub<long,16>(const long N, const long* X);
template long simd_reduction_sub<long,32>(const long N, const long* X);
template long simd_reduction_sub<long,64>(const long N, const long* X);

template int simd_reduction_sub<int,4>(const long N, const int* X);
template int simd_reduction_sub<int,8>(const long N, const int* X);
template int simd_reduction_sub<int,16>(const long N, const int* X);
template int simd_reduction_sub<int,32>(const long N, const int* X);
template int simd_reduction_sub<int,64>(const long N, const int* X);

//------------------------------------------------------------------------------
//  Hand-coded routines for claimed 32 BYTE boundaries on double Xays
#if defined (__AVX2__)
#include <stdio.h>
template<>
double simd_reduction_sub<double,32>(const long N, const double* X)
{
  //using horizonal add
  double local_sub[4];
  local_sub[0] = (double) 0;
  local_sub[1] = (double) 0;
  local_sub[2] = (double) 0;
  local_sub[3] = (double) 0;

  long i=0;

  if (N >= 8) //code below only valid if N is >=8
  {
    const double* p0 = X;
    const double* p1 = X+4;
    //first iteration
    __m256d v0 = _mm256_load_pd(p0);
    __m256d v1 = _mm256_load_pd(p1);
    __m256d a  = _mm256_hadd_pd(v0,v1); //accumulator
    p0+=8;
    p1+=8;

    //SIMD loops
    for (i=8;i<(N-8);i+=8)
    {
      v0  = _mm256_load_pd(p0);
      v1  = _mm256_load_pd(p1);
      a  += _mm256_hadd_pd(v0,v1);
      p0+=8;
      p1+=8;
    }

    _mm256_storeu_pd(&local_sub[0],a);
  }

  //cleanup
  for (i=i;i<N;i++)
  {
    local_sub[0] += *(X+i);
  }

  return (double)(-1)*(local_sub[0] + local_sub[1] + local_sub[2] + local_sub[3]);
}
//------------------------------------------------------------------------------
//  Hand-coded routines for claimed 64 BYTE boundaries on double Xays
//  Same as code for 32 byte boundaries, for now
template<>
double simd_reduction_sub<double,64>(const long N, const double* X)
{
  return simd_reduction_sub<double,32>(N,X);
}
#endif

