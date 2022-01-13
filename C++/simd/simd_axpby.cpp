/* simd_axpby.cpp
 *	JHT, January 11, 2022 : created 
 *
 * .cpp file that implements simd axpby operation 
 * between two continuous sections of data
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * axpby without (known) alignment
 *
 *---------------------------------------------------------------------*/
template <typename T>
void simd_axpby(const long N, const T A, const T* X, const T B, T* Y)
{
  #if defined (_OPENMP)
    T TMP;
    #pragma omp simd  
    for (long i=0;i<N;i++)
    {
      TMP  = A * *(X+i);
      TMP += B * *(Y+i);
      *(Y+i) = TMP;
    }
  #else
    T TMP0;
    T TMP1;
    T TMP2;
    T TMP3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      TMP0  = A * *(X+i+0);
      TMP1  = A * *(X+i+1);
      TMP2  = A * *(X+i+2);
      TMP3  = A * *(X+i+3);
      TMP0 += B * *(Y+i+0); 
      TMP1 += B * *(Y+i+1); 
      TMP2 += B * *(Y+i+2); 
      TMP3 += B * *(Y+i+3); 
      *(Y+i+0) = TMP0; 
      *(Y+i+1) = TMP1; 
      *(Y+i+2) = TMP2; 
      *(Y+i+3) = TMP3; 
    }
    
    for (i=i;i<N;i++)
    {
      TMP0  = A * *(X+i);
      TMP0 += B * *(Y+i);
     *(Y+i) = TMP0;
    }
  #endif
}
template void simd_axpby<double>(const long N, const double A, const double* X, const double B, double* Y);
template void simd_axpby<float>(const long N, const float A, const float* X, const float B, float* Y);
template void simd_axpby<long>(const long N, const long A, const long* X, const long B, long* Y);
template void simd_axpby<int>(const long N, const int A, const int* X, const int B, int* Y);


/*---------------------------------------------------------------------
 * axpby with known alignment 
 *
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
void simd_axpby(const long N, const T A, const T* X, const T B, T* Y)
{
  #if defined (_OPENMP)
    T TMP;
    #pragma omp simd  aligned(X,Y:ALIGNMENT) 
    for (long i=0;i<N;i++)
    {
      TMP  = A * *(X+i);
      TMP += B * *(Y+i);
      *(Y+i) = TMP;
    }
  #else
    T TMP0;
    T TMP1;
    T TMP2;
    T TMP3;
    long i=0;
    for (i=0;i<(N-4);i+=4)
    {
      TMP0  = A * *(X+i+0);
      TMP1  = A * *(X+i+1);
      TMP2  = A * *(X+i+2);
      TMP3  = A * *(X+i+3);
      TMP0 += B * *(Y+i+0); 
      TMP1 += B * *(Y+i+1); 
      TMP2 += B * *(Y+i+2); 
      TMP3 += B * *(Y+i+3); 
      *(Y+i+0) = TMP0; 
      *(Y+i+1) = TMP1; 
      *(Y+i+2) = TMP2; 
      *(Y+i+3) = TMP3; 
    }
    
    for (i=i;i<N;i++)
    {
      TMP0  = A * *(X+i);
      TMP0 += B * *(Y+i);
     *(Y+i) = TMP0;
    }
  #endif
}

template void simd_axpby<double,64>(const long N, const double A, const double* X, const double B, double* Y);
template void simd_axpby<double,32>(const long N, const double A, const double* X, const double B, double* Y);
template void simd_axpby<double,16>(const long N, const double A, const double* X, const double B, double* Y);
template void simd_axpby<double,8>(const long N, const double A, const double* X, const double B, double* Y);

template void simd_axpby<float,64>(const long N, const float A, const float* X, const float B, float* Y);
template void simd_axpby<float,32>(const long N, const float A, const float* X, const float B, float* Y);
template void simd_axpby<float,16>(const long N, const float A, const float* X, const float B, float* Y);
template void simd_axpby<float,8>(const long N, const float A, const float* X, const float B, float* Y);
template void simd_axpby<float,4>(const long N, const float A, const float* X, const float B, float* Y);

template void simd_axpby<long,64>(const long N, const long A, const long* X, const long B, long* Y);
template void simd_axpby<long,32>(const long N, const long A, const long* X, const long B, long* Y);
template void simd_axpby<long,16>(const long N, const long A, const long* X, const long B, long* Y);
template void simd_axpby<long,8>(const long N, const long A, const long* X, const long B, long* Y);

template void simd_axpby<int,64>(const long N, const int A, const int* X, const int B, int* Y);
template void simd_axpby<int,32>(const long N, const int A, const int* X, const int B, int* Y);
template void simd_axpby<int,16>(const long N, const int A, const int* X, const int B, int* Y);
template void simd_axpby<int,8>(const long N, const int A, const int* X, const int B, int* Y);
template void simd_axpby<int,4>(const long N, const int A, const int* X, const int B, int* Y);
