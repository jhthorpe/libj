/*------------------------------------------------------------------
  linal_usym2v.cpp
	JHT, December 21, 2021 : created

  .cpp file for a function which copies an upper symmetric matrix
  (stored upper symmetric only) to a buffer vector which 
  stores both the upper and lower elements, but the lower symm. 
  elements are uninitialized
------------------------------------------------------------------*/
#include "linal_usym2v.hpp"

template<typename T>
void linal_usym2v(const long N, const T* A, T* B)
{
  for (long col=0;col<N;col++)
  {
    simd_copy<T>(col+1,A,B);
    A += col+1;
    B += N;
  }
}
template void linal_usym2v<double>(const long N, const double* A, double* T); 
template void linal_usym2v<float>(const long N, const float* A, float* T); 
template void linal_usym2v<long>(const long N, const long* A, long* T); 
template void linal_usym2v<int>(const long N, const int* A, int* T); 

/*
0 1 3   0 3 6 
  2 4   1 4 7
    5   2 5 8
*/

/*
template<typename T, const int ALIGN>
void linal_usym2v(const long N, const T* A, T* B)
{
  for (long col=0;col<N;col++)
  {
    simd_copy<T,ALIGN>(col+1,A,B);
    A += col+1;
    B += N;
  }
}

template void linal_usym2v<double,64>(const long N, const double* A, double* T); 
template void linal_usym2v<double,32>(const long N, const double* A, double* T); 
template void linal_usym2v<double,16>(const long N, const double* A, double* T); 
template void linal_usym2v<double,8>(const long N, const double* A, double* T); 

template void linal_usym2v<float,64>(const long N, const float* A, float* T); 
template void linal_usym2v<float,32>(const long N, const float* A, float* T); 
template void linal_usym2v<float,16>(const long N, const float* A, float* T); 
template void linal_usym2v<float,8>(const long N, const float* A, float* T); 
template void linal_usym2v<float,4>(const long N, const float* A, float* T); 

template void linal_usym2v<long,64>(const long N, const long* A, long* T); 
template void linal_usym2v<long,32>(const long N, const long* A, long* T); 
template void linal_usym2v<long,16>(const long N, const long* A, long* T); 
template void linal_usym2v<long,8>(const long N, const long* A, long* T); 

template void linal_usym2v<int,64>(const long N, const int* A, int* T); 
template void linal_usym2v<int,32>(const long N, const int* A, int* T); 
template void linal_usym2v<int,16>(const long N, const int* A, int* T); 
template void linal_usym2v<int,8>(const long N, const int* A, int* T); 
template void linal_usym2v<int,4>(const long N, const int* A, int* T); 
*/
