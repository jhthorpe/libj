/*------------------------------------------------------------
  linal_ATDApU.hpp
	JHT, January 1, 2022 : created

  .hpp file for the linal_ATDApU function, which performs
  the following operation:

  U = A^T . alpha*D . A + beta*U

  where U is a upper symmetric matrix (MxM), A is a general 
  (NxM) matrix, and D is a diagonal (NxN) matrix, where

  It is assumed that A,D, and U are stored sequentially 
  in memory, column major, and that only the diagonal 
  elements of D, and the upper triangular (M*(M+1)/2) elements 
  of U are stored

Parameters
M	const long	rows of U,A^T | cols of D,A
N	const long      rows of A     | cols of A^T	
A	const T*	A matrix (NxM)
ALPHA	const T		constant multipling D 
D	const T*	D diagonal elements (N)
BETA	const T		beta constant to multiply U
U	T*		U upper triangular elements (M*(M+1)/2)	

------------------------------------------------------------*/
#ifndef LINAL_ATDApU_HPP
#define LINAL_ATDApU_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template<typename T>
void linal_ATDApU(const long M, const long N, const T* A, const T ALPHA, const T* D, const T BETA, T* U);

/*
template<typename T, const int ALIGN>
void linal_ATDApU(const long M, const long N, const T* A, const T ALPHA, const T* D, const T BETA, T* U);
*/
#endif
