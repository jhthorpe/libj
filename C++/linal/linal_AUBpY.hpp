/*-----------------------------------------------------------
  linal_AUBpY.cpp
        JHT, January 7, 2022 : created

  .cpp file for AUBpY, which matrix multiplies a
  MxL matrix A (which may be scaled), 
  and LxL upper symmetric matrix U, a LxN matrix B, 
  and adds the result to a (scaled) MxM upper triangular matrix Y
  
  **IN PLACE ALGORITHM** 

  It is assumed that A,U,B, and Y are held continuously 
  in memory, that A,U,B, and Y are stored column major, and that 
  only the upper triangular elements of U and Y are stored.
  
            A           U          B                  Y
         _______    ________     _____             ______
        | 0 2 4 |   \ 0 1 3 |   | 0 3 |            \ 0 1 |
alpha*  | . . . |     \ . . |   | . . |  += beta *   \ . |
         _______        \ . |   | . . |               ---
                  .      ---  .  -----             

This algorithm proceeds by constructing the intermediate elements of U.B,
 and distributing them along the rows of A

  This uses the following equations:

  Y(i,j) = sum_k A(i,k)*I(k,j)
  I(k,j) = sum_l U(k,l)*B(l,j)

This could be improved by blocking, but we will do this at
a later date

Parameters:
M       const long      rows of A,Y, cols of B
L       const long      cols of A,rows of U,B
ALPHA   cosnt T         value to scale A with
A*      const T*        pointer to A
U*      const T*        pointer to U
B*      const T*        pointer to B
BETA    const T         value to scale Y with
Y*      T*              pointer to Y

-----------------------------------------------------------*/
#ifndef LINAL_AUBPY_HPP
#define LINAL_AUBPY_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template <typename T>
void linal_AUBpY(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* Y);

/*
template <typename T, const int ALIGN>
void linal_AUBpY(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* Y);
*/

#endif
