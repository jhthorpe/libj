/*-----------------------------------------------------------
  linal_AUBpC.cpp
	JHT, January 9, 2022 : created

  .cpp file for AUBpC, which matrix multiplies a
  MxL matrix A (which may be scaled), 
  and LxL upper symmetric matrix U, a LxN matrix B, 
  and adds the result to a (scaled) MxN matrix C
  
  **IN PLACE ALGORITHM** 

  It is assumed that A,U,B, and C are held continuously 
  in memory, that A,Y,B, and Y are stored column major, and that 
  only the upper triangular elements of U and Y are stored.
  
           A            U           B                    C
         _______    ________     _______              _______
        | 0 2 4 |   \ 0 1 3 |   | 0 3 6 |            | 0 2 4 |
alpha*  | . . . |     \ . . |   | . . . |  += beta * | . . . |
         _______        \ . |   | . . . |             -------
                  .      ---  .  -------             

This algorithm proceeds by constructing the intermediate elements of U.B,
 and distributing them along the rows of A

This could be improved by blocking, but we will do this at
a later date

Parameters:
M	const long	rows of A,C
N	const long      cols of B,C
L	const long	cols of A,rows of U,B
ALPHA	cosnt T		value to scale A with
A*	const T*	pointer to A
U*	const T*   	pointer to U
B*	const T*	pointer to B
BETA	const T		value to scale Y with
C*	T*		pointer to C

-----------------------------------------------------------*/
#ifndef LINAL_AUBPC_HPP
#define LINAL_AUBPC_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template <typename T>
void linal_AUBpC(const long M, const long N, const long L,
                 const T ALPHA, const T* A, const T* U, 
                 const T* B, const T BETA, T* C);

/*
template <typename T, const int ALIGN>
void linal_AUBpC(const long M, const long N, const long L,
                 const T ALPHA, const T* A, const T* U, 
                 const T* B, const T BETA, T* C);
*/

#endif
