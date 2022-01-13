/*----------------------------------------------------------------
  linal_usym3_sqm3_MM_UP.hpp
	JHT, September 1, 2021

  Inverts an symmetric 3x3 matrix, which is assumed to be
  stored upper triangular.
-----------------------------------------------------------------*/
#ifndef LINAL_USYM3_SQM3_MM_UP_HPP
#define LINAL_USYM3_SQM3_MM_UP_HPP
template <typename T>
void linal_usym3_sqm3_MM_UP(const T* A, const T* B, T* C);
#endif
