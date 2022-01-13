/*-------------------------------------------------
  linal_ATpB.hpp
	JHT, January 7, 2022 : created

  .hpp file for ATpB, which transposes an NxM
  matrix A, scales it, and adds it to a scaled
  MxN matrix B 

  It is assumed that A and B are stored sequentially
  in memory

Parameters
-------------------
M	const long	rows of B, cols of A
N	const long	cols of B, rows of A
ALPHA	const T		constant to multiply A^T
A*	const T*	pointer to A (NxM)
BETA	const T		constant to multiply B
B*	T*		pointer to B matrix (MxN)		

-------------------------------------------------*/
#ifndef LINAL_ATPB_HPP
#define LINAL_ATPB_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template<typename T>
void linal_ATpB(const long M, const long N, const T ALPHA, 
                const T* A, const T BETA, T* B);

/*
template<typename T, const int ALIGN>
void linal_ATpB(const long M, const long N, const T ALPHA, 
                const T* A, const T BETA, T* B);
*/

#endif
