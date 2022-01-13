/*-----------------------------------------------------------
  linal_DATpB.hpp
	JHT, December 30, 2021 : created

  .hpp file for linal_DATpB, which multiplies a scaled diagonal
  matrix (D) of M elements against the transpose of a matrix
  A^T of dimension (MxN) and adds the result to the scaled
  matrix B (MxN) 

  B = alpha*D*A^T + beta*B

Parameters
M	long		rows of D,B,A^T
N	long		cols of B,A^T
ALPHA	const T		constant to multiple D
D	const T* 	vector of diagonal matrix (M)
A	const T*	matrix to multiply (NxM)
BETA	const T		constant to multiply B
B	T*		resulting matrix (MxN)

This routine could probably use with a blocking algorithm

-----------------------------------------------------------*/
#ifndef LINAL_DATpB_HPP
#define LINAL_DATpB_HPP

#include "linal_def.hpp"
#include "simd.hpp"
#include <math.h>

template<typename T>
void linal_DATpB(const long M, const long N, const T ALPHA, const T* D, 
                const T* A, const T BETA, T* B);

#endif
