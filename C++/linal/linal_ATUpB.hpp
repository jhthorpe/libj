/*--------------------------------------------------
  linal_ATUpB.hpp
	JHT, December 20, 2021 : created

  .cpp file for the linal_ATUpB, which multiplies
   an MxN matrix, A, by a symmetric NxN matrix U, 
   and stores the result in an MxN matrix B. Matrices
   A and C can optionally be scaled by some constant, 
   alpha and beta, respectively:
 
   B = alpha*A^T*U + beta*B
 
   It is assumed that only the upper-triangular 
   elements of the matrix U are stored, and that
   all matrices are sequential in memory
 
 Parameters
 M       long            number of rows
 N       long            number of cols
 alpha   const T         scalar to multiply A
 A       const T*        matrix A (MxN)
 U       const T*        matrix U ((NxN+1)/2)
 beta    const T	 scalar to multiply C 
 B       T*              matrix B

--------------------------------------------------*/
#ifndef LINAL_ATUPB_HPP
#define LINAL_ATUPB_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template<typename T>
void linal_ATUpB(const long M, const long N, const T ALPHA, 
                 const T* A,const T* U, const T BETA, T* C);

/*
template<typename T,const int ALIGN>
void linal_ATUpB(const long M, const long N, const T ALPHA, 
                 const T* A,const T* U, const T BETA, T* C);
*/
#endif
