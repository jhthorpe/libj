/*------------------------------------------------
  linal_DApB.hpp
    JHT, August 18, 2021

  Multiplies a scaled diagonal matriD, D (MxM), 
  by a rectangular matrix A(MxN), and adds this to 
  a scaled matrix B (MxN). It is assumed  
  that all elements are stored sequentially in memory.

  B = alpha*D.A + beta*B 

  Parameters
  M     long            Rows of D,A,B
  N     long            Cols of A,B
  ALPHA const T         alpha constant multiplies D
  D     const T*        Diagonal elements of X(M),       
  A     const T*        A Matrix (MxN)
  BETA  const T         beta constant multiplies B      
  B	T*		B matrix (MxN)
  
------------------------------------------------*/
#ifndef LINAL_DAPB_HPP
#define LINAL_DAPB_HPP

#include <math.h>
#include "linal_def.hpp"
#include "simd.hpp"
template <typename T>
void linal_DApB(const long M, const long N, const T ALPHA, const T* D, const T* A, const T BETA, T* B); 
/*
template <typename T, const int ALIGN>
void linal_DApB(const long M, const long N, const T ALPHA, const T* D, const T* A, const T BETA, T* B); 
*/

#endif
