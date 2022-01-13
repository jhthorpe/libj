/*---------------------------------------------------------
 * raxmy
 *
 * take the reciprocal of vector x, scales it by constant a
 * and multiplies the result into vector y 
 * 
 *
 *  simd_raxmy<type [,align]>(const long N, const type a, const type* X, ty
pe* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on      
 *  A      -> scalar to multiply w*x by 
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/


#include "simd.hpp"

/*---------------------------------------------------------------------
 * raxmy without (known) alignment
 *
 *---------------------------------------------------------------------*/
template <typename T>
void simd_raxmy(const long N, const T A, const T* X, T* Y)
{
  const T ONE = (T) 1;
  #if defined (_OPENMP)
    T TMP;
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      TMP = ONE / *(X+i); 
      *(Y+i) *= A * TMP; 
    }
  #else
    T TMP0;
    T TMP1;
    T TMP2;
    T TMP3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      TMP0 = ONE / *(X+i+0);
      TMP1 = ONE / *(X+i+1);
      TMP2 = ONE / *(X+i+2);
      TMP3 = ONE / *(X+i+3);
      *(Y+i+0) *= A * TMP0; 
      *(Y+i+1) *= A * TMP1; 
      *(Y+i+2) *= A * TMP2; 
      *(Y+i+3) *= A * TMP3; 
    }
    
    for (i=i;i<N;i++)
    {
      TMP0 = ONE / *(X+i);
      *(Y+i) *= A * TMP0; 
    }
  #endif
}
template void simd_raxmy<double>(const long N, const double A, const double* X, double* Y);
template void simd_raxmy<float>(const long N, const float A, const float* X, float* Y);
template void simd_raxmy<long>(const long N, const long A, const long* X, long* Y);
template void simd_raxmy<int>(const long N, const int A, const int* X, int* Y);


/*---------------------------------------------------------------------
 * raxmy with known alignment 
 *
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_raxmy(const long N, const T A, const T* X, T* Y)
{
  const T ONE = (T) 1;
  #if defined (_OPENMP)
    T TMP;
    #pragma omp simd  aligned(X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      TMP = ONE / *(X+i); 
      *(Y+i) *= A * TMP; 
    }
  #else
    T TMP0;
    T TMP1;
    T TMP2;
    T TMP3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      TMP0 = ONE / *(X+i+0);
      TMP1 = ONE / *(X+i+1);
      TMP2 = ONE / *(X+i+2);
      TMP3 = ONE / *(X+i+3);
      *(Y+i+0) *= A * TMP0; 
      *(Y+i+1) *= A * TMP1; 
      *(Y+i+2) *= A * TMP2; 
      *(Y+i+3) *= A * TMP3; 
    }
    
    for (i=i;i<N;i++)
    {
      TMP0 = ONE / *(X+i);
      *(Y+i) *= A * TMP0; 
    }
  #endif
}

template void simd_raxmy<double,64>(const long N, const double A, const double* X, double* Y);
template void simd_raxmy<double,32>(const long N, const double A, const double* X, double* Y);
template void simd_raxmy<double,16>(const long N, const double A, const double* X, double* Y);
template void simd_raxmy<double,8>(const long N, const double A, const double* X, double* Y);

template void simd_raxmy<float,64>(const long N, const float A, const float* X, float* Y);
template void simd_raxmy<float,32>(const long N, const float A, const float* X, float* Y);
template void simd_raxmy<float,16>(const long N, const float A, const float* X, float* Y);
template void simd_raxmy<float,8>(const long N, const float A, const float* X, float* Y);
template void simd_raxmy<float,4>(const long N, const float A, const float* X, float* Y);

template void simd_raxmy<long,64>(const long N, const long A, const long* X, long* Y);
template void simd_raxmy<long,32>(const long N, const long A, const long* X, long* Y);
template void simd_raxmy<long,16>(const long N, const long A, const long* X, long* Y);
template void simd_raxmy<long,8>(const long N, const long A, const long* X, long* Y);

template void simd_raxmy<int,64>(const long N, const int A, const int* X, int* Y);
template void simd_raxmy<int,32>(const long N, const int A, const int* X, int* Y);
template void simd_raxmy<int,16>(const long N, const int A, const int* X, int* Y);
template void simd_raxmy<int,8>(const long N, const int A, const int* X, int* Y);
template void simd_raxmy<int,4>(const long N, const int A, const int* X, int* Y);
