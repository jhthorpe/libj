/*-----------------------------------------------------------
  linal_AUBpD.cpp
        JHT, January 9, 2022 : created

  .cpp file for AUBpD, which matrix multiplies a
  MxL matrix A (which may be scaled), 
  and LxL upper symmetric matrix U, a LxN matrix B, 
  and adds the diagonal elements of the resulting matrix 
  to a MxM diagonal matrix D 
   
  
  **IN PLACE ALGORITHM** 

  **This proceeds by constructing U.B, then distributing A**
  **If A is smaller than B, another algorithm should be used**

  It is assumed that A,U,B, and D are held continuously 
  in memory, that A,U,B, and D are stored column major, that 
  only the upper triangular elements of U and the diagonal 
  elements of D are stored.

            A           U          B                  D
         _______    ________     _____              _____
        | 0 2 4 |   \ 0 1 3 |   | 0 3 |            | 0   |
alpha*  | . . . |     \ . . |   | . . |  += beta * |   . |
         _______        \ . |   | . . |             -----
                  .      ---  .  -----             

  
This algorithm proceeds by constructing the intermediate elements of U.B,
 and distributing them along the rows of A. 

  This uses the following equations:

  D(j,j) = sum_k A(i,k)*I(k,j)
  I(k,j) = sum_l U(k,l)*B(l,j)

This could be improved by blocking, but we will do this at
a later date

Parameters:
M       const long      rows of A,D, cols of B
L       const long      cols of A,rows of U,B
ALPHA   cosnt T         value to scale A with
A*      const T*        pointer to A
U*      const T*        pointer to U
B*      const T*        pointer to B
BETA    const T         value to scale D with
D*      T*              pointer to D


-----------------------------------------------------------*/
#include "linal_def.hpp"
#include "simd.hpp"
#include <math.h>

template <typename T>
void linal_AUBpD(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* D);

/*
template <typename T,const int ALIGN>
void linal_AUBpD(const long M, const long L, const T ALPHA, 
                  const T* A, const T* U, const T* B,
                  const T BETA, T* D);
*/

