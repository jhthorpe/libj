/* simd_copy.cpp
 * JHT, December 9, 2021 : creaed 
 *
 * .cpp file that implements simd copy operation of a vector
 * X into vector Y.
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"
#include <cstring>

/*---------------------------------------------------------------------
 * copy (without known alignment) 
 *
 *---------------------------------------------------------------------*/
template <typename T>
void simd_copy(const long N, const T* X, T* Y)
{
  //After some testing, I've found that memcpy is just faster...
  std::memcpy(Y,X,N*sizeof(T));

  /*
  #if defined (_OPENMP)
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      *(Y+i) = *(X+i); 
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(Y+i+0) = *(X+i+0);
      *(Y+i+1) = *(X+i+1); 
      *(Y+i+2) = *(X+i+2); 
      *(Y+i+3) = *(X+i+3); 
    }
    
    for (i=i;i<N;i++)
    {
      *(Y+i) = *(X+i); 
    }
  #endif
  */
}
template void simd_copy<double>(const long N, const double* X, double* Y);
template void simd_copy<float>(const long N, const float* X, float* Y);
template void simd_copy<long>(const long N, const long* X, long* Y);
template void simd_copy<int>(const long N, const int* X, int* Y);


/*---------------------------------------------------------------------
 * copy with known alignment 
 *
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_copy(const long N, const T* X, T* Y)
{
  //After some testing, I've found that memcpy is just faster...
  std::memcpy(Y,X,N*sizeof(T));

  /*
  #if defined (_OPENMP)
    #pragma omp simd  aligned(X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      *(Y+i) = *(X+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(Y+i+0) = *(X+i+0);
      *(Y+i+1) = *(X+i+1);
      *(Y+i+2) = *(X+i+2);
      *(Y+i+3) = *(X+i+3);
    }
    
    for (i=i;i<N;i++)
    {
      *(Y+i) = *(X+i);
    }
  #endif
  */
}

#if !defined (__AVX2__) //special instructions below
template void simd_copy<double,128>(const long N, const double* X, double* Y);
template void simd_copy<double,64>(const long N, const double* X, double* Y);
template void simd_copy<double,32>(const long N, const double* X, double* Y);
#endif
template void simd_copy<double,16>(const long N, const double* X, double* Y);
template void simd_copy<double,8>(const long N, const double* X, double* Y);

template void simd_copy<float,128>(const long N, const float* X, float* Y);
template void simd_copy<float,64>(const long N, const float* X, float* Y);
template void simd_copy<float,32>(const long N, const float* X, float* Y);
template void simd_copy<float,16>(const long N, const float* X, float* Y);
template void simd_copy<float,8>(const long N, const float* X, float* Y);
template void simd_copy<float,4>(const long N, const float* X, float* Y);

template void simd_copy<long,128>(const long N, const long* X, long* Y);
template void simd_copy<long,64>(const long N, const long* X, long* Y);
template void simd_copy<long,32>(const long N, const long* X, long* Y);
template void simd_copy<long,16>(const long N, const long* X, long* Y);
template void simd_copy<long,8>(const long N, const long* X, long* Y);

template void simd_copy<int,128>(const long N, const int* X, int* Y);
template void simd_copy<int,64>(const long N, const int* X, int* Y);
template void simd_copy<int,32>(const long N, const int* X, int* Y);
template void simd_copy<int,16>(const long N, const int* X, int* Y);
template void simd_copy<int,8>(const long N, const int* X, int* Y);
template void simd_copy<int,4>(const long N, const int* X, int* Y);


/*---------------------------------------------------------------------
 * copy of doubles with alignment for AVX2

---------------------------------------------------------------------*/
#if defined (__AVX2__)
template<>
void simd_copy<double,32>(const long N, const double* X, double* Y)
{
  //After testing, I have found that memcpy is faster
  std::memcpy(Y,X,N*sizeof(double));

  /*
  long i=0;
  if (N >= 8) 
  {
    const double* p0 = X;
    const double* p1 = X+4;
    double* q0 = Y;
    double* q1 = Y+4;

    //first iteration
    __m256d x0 = _mm256_load_pd(p0);
    __m256d x1 = _mm256_load_pd(p1);
    _mm256_store_pd(q0,x0);
    _mm256_store_pd(q1,x1);
    p0+=8;
    p1+=8;
    q0+=8;
    q1+=8;

    //SIMD loops
    for (i=8;i<(N-8);i+=8)
    {
      x0 = _mm256_load_pd(p0);
      x1 = _mm256_load_pd(p1);
      _mm256_store_pd(q0,x0);
      _mm256_store_pd(q1,x1);
      p0+=8;
      p1+=8;
      q0+=8;
      q1+=8;
    }
  }

  //cleanup
  for (i=i;i<N;i++)
  {
    *(Y+i) = *(X+i);
  }
  */
}
//------------------------------------------------------------------------------
//  Hand-coded routines for claimed 64,128 BYTE boundaries on double Xays
//  Same as code for 32 byte boundaries, for now
template<>
void simd_copy<double,64>(const long N, const double* X, double* Y)
{
  simd_copy<double,32>(N,X,Y);
}

template<>
void simd_copy<double,128>(const long N, const double* X, double* Y)
{
  simd_copy<double,32>(N,X,Y);
}
#endif

