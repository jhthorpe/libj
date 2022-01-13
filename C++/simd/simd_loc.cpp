/* simd_loc.cpp
 * JHT, December 11, 2021 : created 
 *
 * .cpp file that implements simd identification of the 
 * first element in an array that matches an input value 
 *
 * If compiled with OpenMP, will use the OpenMP SIMD pragmas to 
 * provide hints to the compiler
 *
 */

#include "simd.hpp"

/*---------------------------------------------------------------------
 * loc without (known) alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *---------------------------------------------------------------------*/
template <typename T>
long simd_loc(const long N, const T A, const T* X)
{
  long i=0;
  for (i=0;i<(N-4);i+=4)
  {
    if (*(X+i) == A)   {return i;}
    if (*(X+i+1) == A) {return i+1;}
    if (*(X+i+2) == A) {return i+2;}
    if (*(X+i+3) == A) {return i+3;}
  }
    
  for (i=i;i<N;i++)
  {
    if (*(X+i) == A) {return i;}
  }
  return -1;
}
template long simd_loc<long>(const long N, const long A, const long* X);
template long simd_loc<int>(const long N, const int A, const int* X);


/*---------------------------------------------------------------------
 * loc with known alignment
 *   - if OpenMP is defined, use the pragmas to request vectorization
 *   - otherwise, use loop unrolling
 *
 * Future versions will use specifically vectorized code
 *---------------------------------------------------------------------*/
template <typename T, const int ALIGNMENT>
long simd_loc(const long N, const T A, const T* X)
{
  long i=0;
  for (i=0;i<(N-4);i+=4)
  {
    if (*(X+i) == A)   {return i;}
    if (*(X+i+1) == A) {return i+1;}
    if (*(X+i+2) == A) {return i+2;}
    if (*(X+i+3) == A) {return i+3;}
  }
    
  for (i=i;i<N;i++)
  {
    if (*(X+i) == A) {return i;}
  }
  return -1;
}

template long simd_loc<long,128>(const long N, const long A, const long* X);
template long simd_loc<long,64>(const long N, const long A, const long* X);
template long simd_loc<long,32>(const long N, const long A, const long* X);
template long simd_loc<long,16>(const long N, const long A, const long* X);
template long simd_loc<long,8>(const long N, const long A, const long* X);

template long simd_loc<int,128>(const long N, const int A, const int* X);
template long simd_loc<int,64>(const long N, const int A, const int* X);
template long simd_loc<int,32>(const long N, const int A, const int* X);
template long simd_loc<int,16>(const long N, const int A, const int* X);
template long simd_loc<int,8>(const long N, const int A, const int* X);
template long simd_loc<int,4>(const long N, const int A, const int* X);
