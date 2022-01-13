/* simd_dotwxy.cpp
 *	JHT, January 1, 2022 : created
 *
 * .cpp file that implements simd dot-product operation 
 * between three continuous sections of data
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
T simd_dotwxy(const long N, const T* W, const T* X, const T* Y)
{
  T dot = 0;
  #if defined (_OPENMP)
    T tmp;
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      tmp  = *(W+i) * *(X+i);
      dot += *(Y+i) * tmp; 
      
    }
  #else
    T tmp0;
    T tmp1;
    T tmp2;
    T tmp3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      tmp0 = *(W+i+0) * *(X+i+0);
      tmp1 = *(W+i+1) * *(X+i+1);
      tmp2 = *(W+i+2) * *(X+i+2);
      tmp3 = *(W+i+3) * *(X+i+3);

      dot += *(Y+i+0) * tmp0; 
      dot += *(Y+i+1) * tmp1; 
      dot += *(Y+i+2) * tmp2; 
      dot += *(Y+i+3) * tmp3; 
    }
    
    for (i=i;i<N;i++)
    {
      tmp0 = *(W+i) * *(X+i);
      dot += *(Y+i) * tmp0; 
    }
  #endif
  return dot;
}
template double simd_dotwxy<double>(const long N, const double* W, const double* X, const double* Y);
template float simd_dotwxy<float>(const long N, const float* W, const float* X, const float* Y);
template long simd_dotwxy<long>(const long N, const long* W, const long* X, const long* Y);
template int simd_dotwxy<int>(const long N, const int* W, const int* X, const int* Y);


/*---------------------------------------------------------------------
 * Element wise addition with known alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *
 * Future versions will use specifically vectorized code
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
T simd_dotwxy(const long N, const T* W, const T* X, const T* Y)
{
  T dot = 0;
  #if defined (_OPENMP)
    T tmp;
    #pragma omp simd  aligned(W,X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      tmp  = *(W+i) * *(X+i);
      dot += *(Y+i) * tmp; 
    }
  #else
    T tmp0;
    T tmp1;
    T tmp2;
    T tmp3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      tmp0 = *(W+i+0) * *(X+i+0);
      tmp1 = *(W+i+1) * *(X+i+1);
      tmp2 = *(W+i+2) * *(X+i+2);
      tmp3 = *(W+i+3) * *(X+i+3);

      dot += *(Y+i+0) * tmp0; 
      dot += *(Y+i+1) * tmp1; 
      dot += *(Y+i+2) * tmp2; 
      dot += *(Y+i+3) * tmp3; 
    }
    
    for (i=i;i<N;i++)
    {
      tmp0 = *(W+i) * *(X+i);
      dot += *(Y+i) * tmp0; 
    }
  #endif
  return dot;
}

template double simd_dotwxy<double,128>(const long N, const double* W, const double* X, const double* Y);
template double simd_dotwxy<double,64>(const long N, const double* W, const double* X, const double* Y);
template double simd_dotwxy<double,32>(const long N, const double* W, const double* X, const double* Y);
template double simd_dotwxy<double,16>(const long N, const double* W, const double* X, const double* Y);
template double simd_dotwxy<double,8>(const long N, const double* W, const double* X, const double* Y);

template float simd_dotwxy<float,128>(const long N, const float* W, const float* X, const float* Y);
template float simd_dotwxy<float,64>(const long N, const float* W, const float* X, const float* Y);
template float simd_dotwxy<float,32>(const long N, const float* W, const float* X, const float* Y);
template float simd_dotwxy<float,16>(const long N, const float* W, const float* X, const float* Y);
template float simd_dotwxy<float,8>(const long N, const float* W, const float* X, const float* Y);
template float simd_dotwxy<float,4>(const long N, const float* W, const float* X, const float* Y);

template long simd_dotwxy<long,128>(const long N, const long* W, const long* X, const long* Y);
template long simd_dotwxy<long,64>(const long N, const long* W, const long* X, const long* Y);
template long simd_dotwxy<long,32>(const long N, const long* W, const long* X, const long* Y);
template long simd_dotwxy<long,16>(const long N, const long* W, const long* X, const long* Y);
template long simd_dotwxy<long,8>(const long N, const long* W, const long* X, const long* Y);

template int simd_dotwxy<int,128>(const long N, const int* W, const int* X, const int* Y);
template int simd_dotwxy<int,64>(const long N, const int* W, const int* X, const int* Y);
template int simd_dotwxy<int,32>(const long N, const int* W, const int* X, const int* Y);
template int simd_dotwxy<int,16>(const long N, const int* W, const int* X, const int* Y);
template int simd_dotwxy<int,8>(const long N, const int* W, const int* X, const int* Y);
template int simd_dotwxy<int,4>(const long N, const int* W, const int* X, const int* Y);
