/* simd_awxpy.cpp
 *	JHT, January 11, 2022 : created
 * element wise multiplies vectors w and x, scales them, and
 * adds the result to a vector y 
 *
 *  simd_awxpy<type [,align]>(const long N, const type a, const type* W, co
nst type* X, type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on      
 *  A      -> scalar to multiply w*x by 
 *  W*     -> Address of first element of W to act on
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/


#include "simd.hpp"

/*---------------------------------------------------------------------
 * awxpy without (known) alignment
 *
 *---------------------------------------------------------------------*/
template <typename T>
void simd_awxpy(const long N, const T A, const T* W, const T* X, T* Y)
{
  #if defined (_OPENMP)
    T TMP;
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      TMP = *(W+i) * *(X+i);
      *(Y+i) += A * TMP; 
    }
  #else
    T TMP0;
    T TMP1;
    T TMP2;
    T TMP3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
       TMP0 = *(W+i+0) * *(X+i+0);
       TMP1 = *(W+i+1) * *(X+i+1);
       TMP2 = *(W+i+2) * *(X+i+2);
       TMP3 = *(W+i+3) * *(X+i+3);
      *(Y+i+0) += A * TMP0;
      *(Y+i+1) += A * TMP1; 
      *(Y+i+2) += A * TMP2; 
      *(Y+i+3) += A * TMP3; 
    }
    
    for (i=i;i<N;i++)
    {
      TMP0 = *(W+i) * *(X+i);
      *(Y+i) += A * TMP0; 
    }
  #endif
}
template void simd_awxpy<double>(const long N, const double A, const double* W, const double* X, double* Y);
template void simd_awxpy<float>(const long N, const float A, const float* W, const float* X, float* Y);
template void simd_awxpy<long>(const long N, const long A, const long* W, const long* X, long* Y);
template void simd_awxpy<int>(const long N, const int A, const int* W, const int* X, int* Y);


/*---------------------------------------------------------------------
 * awxpy with known alignment 
 *
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_awxpy(const long N, const T A, const T* W, const T* X, T* Y)
{
  #if defined (_OPENMP)
    T TMP;
    #pragma omp simd  aligned(W,X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      TMP = *(W+i) * *(X+i);
      *(Y+i) += A * TMP; 
    }
  #else
    T TMP0;
    T TMP1;
    T TMP2;
    T TMP3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
       TMP0 = *(W+i+0) * *(X+i+0);
       TMP1 = *(W+i+1) * *(X+i+1);
       TMP2 = *(W+i+2) * *(X+i+2);
       TMP3 = *(W+i+3) * *(X+i+3);
      *(Y+i+0) += A * TMP0;
      *(Y+i+1) += A * TMP1; 
      *(Y+i+2) += A * TMP2; 
      *(Y+i+3) += A * TMP3; 
    }
    
    for (i=i;i<N;i++)
    {
      TMP0 = *(W+i) * *(X+i);
      *(Y+i) += A * TMP0; 
    }
  #endif
}

template void simd_awxpy<double,64>(const long N, const double A, const double* W, const double* X, double* Y);
template void simd_awxpy<double,32>(const long N, const double A, const double* W, const double* X, double* Y);
template void simd_awxpy<double,16>(const long N, const double A, const double* W, const double* X, double* Y);
template void simd_awxpy<double,8>(const long N, const double A, const double* W, const double* X, double* Y);

template void simd_awxpy<float,64>(const long N, const float A, const float* W, const float* X, float* Y);
template void simd_awxpy<float,32>(const long N, const float A, const float* W, const float* X, float* Y);
template void simd_awxpy<float,16>(const long N, const float A, const float* W, const float* X, float* Y);
template void simd_awxpy<float,8>(const long N, const float A, const float* W, const float* X, float* Y);
template void simd_awxpy<float,4>(const long N, const float A, const float* W, const float* X, float* Y);

template void simd_awxpy<long,64>(const long N, const long A, const long* W, const long* X, long* Y);
template void simd_awxpy<long,32>(const long N, const long A, const long* W, const long* X, long* Y);
template void simd_awxpy<long,16>(const long N, const long A, const long* W, const long* X, long* Y);
template void simd_awxpy<long,8>(const long N, const long A, const long* W, const long* X, long* Y);

template void simd_awxpy<int,64>(const long N, const int A, const int* W, const int* X, int* Y);
template void simd_awxpy<int,32>(const long N, const int A, const int* W, const int* X, int* Y);
template void simd_awxpy<int,16>(const long N, const int A, const int* W, const int* X, int* Y);
template void simd_awxpy<int,8>(const long N, const int A, const int* W, const int* X, int* Y);
template void simd_awxpy<int,4>(const long N, const int A, const int* W, const int* X, int* Y);
