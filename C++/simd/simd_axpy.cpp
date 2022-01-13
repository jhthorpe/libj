/* simd_axpy.cpp
 * JHT, November 15, 2021 : created
 *
 * .cpp file that implements simd axpy operation 
 * between two continuous sections of data
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * axpy without (known) alignment
 *
 *---------------------------------------------------------------------*/
template <typename T>
void simd_axpy(const long N, const T A, const T* X, T* Y)
{
  #if defined (_OPENMP)
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      *(Y+i) += A * *(X+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(Y+i+0) += A * *(X+i+0);
      *(Y+i+1) += A * *(X+i+1); 
      *(Y+i+2) += A * *(X+i+2); 
      *(Y+i+3) += A * *(X+i+3); 
    }
    
    for (i=i;i<N;i++)
    {
      *(Y+i) += A * *(X+i); 
    }
  #endif
}
template void simd_axpy<double>(const long N, const double A, const double* X, double* Y);
template void simd_axpy<float>(const long N, const float A, const float* X, float* Y);
template void simd_axpy<long>(const long N, const long A, const long* X, long* Y);
template void simd_axpy<int>(const long N, const int A, const int* X, int* Y);


/*---------------------------------------------------------------------
 * axpy with known alignment 
 *
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_axpy(const long N, const T A, const T* X, T* Y)
{
  #if defined (_OPENMP)
    #pragma omp simd  aligned(X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      *(Y+i) += A * *(X+i);
    }
  #else
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      *(Y+i+0) += A * *(X+i+0);
      *(Y+i+1) += A * *(X+i+1);
      *(Y+i+2) += A * *(X+i+2);
      *(Y+i+3) += A * *(X+i+3);
    }
    
    for (i=i;i<N;i++)
    {
      *(Y+i) += A * *(X+i);
    }
  #endif
}

template void simd_axpy<double,128>(const long N, const double A, const double* X, double* Y);
template void simd_axpy<double,64>(const long N, const double A, const double* X, double* Y);
template void simd_axpy<double,32>(const long N, const double A, const double* X, double* Y);
template void simd_axpy<double,16>(const long N, const double A, const double* X, double* Y);
template void simd_axpy<double,8>(const long N, const double A, const double* X, double* Y);

template void simd_axpy<float,128>(const long N, const float A, const float* X, float* Y);
template void simd_axpy<float,64>(const long N, const float A, const float* X, float* Y);
template void simd_axpy<float,32>(const long N, const float A, const float* X, float* Y);
template void simd_axpy<float,16>(const long N, const float A, const float* X, float* Y);
template void simd_axpy<float,8>(const long N, const float A, const float* X, float* Y);
template void simd_axpy<float,4>(const long N, const float A, const float* X, float* Y);

template void simd_axpy<long,128>(const long N, const long A, const long* X, long* Y);
template void simd_axpy<long,64>(const long N, const long A, const long* X, long* Y);
template void simd_axpy<long,32>(const long N, const long A, const long* X, long* Y);
template void simd_axpy<long,16>(const long N, const long A, const long* X, long* Y);
template void simd_axpy<long,8>(const long N, const long A, const long* X, long* Y);

template void simd_axpy<int,128>(const long N, const int A, const int* X, int* Y);
template void simd_axpy<int,64>(const long N, const int A, const int* X, int* Y);
template void simd_axpy<int,32>(const long N, const int A, const int* X, int* Y);
template void simd_axpy<int,16>(const long N, const int A, const int* X, int* Y);
template void simd_axpy<int,8>(const long N, const int A, const int* X, int* Y);
template void simd_axpy<int,4>(const long N, const int A, const int* X, int* Y);
