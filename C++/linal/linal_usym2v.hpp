/*------------------------------------------------------------------
  linal_usym2v.hpp
	JHT, December 21, 2021 : created

  .hpp file for a function which copies an upper symmetric matrix
  (stored upper symmetric only) to a buffer vector which 
  stores both the upper and lower elements, but the lower symm. 
  elements are uninitialized
------------------------------------------------------------------*/
#ifndef LINAL_USYM2V_HPP
#define LINAL_USYM2V_HPP

#include "simd.hpp"

template<typename T>
void linal_usym2v(const long N, const T* A, T* B);

/*
template<typename T, const int ALIGN>
void linal_usym2v(const long N, const T* A, T* B);
*/

#endif
