/*----------------------------------------------------------------
  linal_usym3_usym3_MM.hpp
	JHT, September 1, 2021

  Inverts an symmetric 3x3 matrix, which is assumed to be
  stored upper triangular.
-----------------------------------------------------------------*/
#ifndef LINAL_USYM3_USYM3_MM_HPP
#define LINAL_USYM3_USYM3_MM_HPP
template <typename T>
void linal_usym3_usym3_MM(const T* A, const T* B, T* C);
#endif
