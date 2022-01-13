/*-----------------------------------------------------------
  linal_ATUxpy.hpp
	JHT, January 7, 2022 : created

  .hpp file for ATUxpy, which matrix multiplies the 
  transpose of an NxM matrix A (which may be scaled), 
  and NxN upper symmetric matrix U, a N vector x, and adds
  them to a (scaled) M vector y  **IN PLACE** 

  It is assumed that A,U,v, and y are held continuously 
  in memory, that A is stored column major, and that
  U is 
  
           A^T          U         x                y
         ______     ________     ___              ___
        | 0 3 5 |   \ 0 1 3 |   | 0 |            | 0 |
alpha*  | . . . |     \ . . |   | . |  += beta * | . |
        | . . . |       \ . |   | . |            | . |
         ------   .      ---  .  ---              ---

  For better performance, it is recomended to use an 
  intermediate array to store the result of U.x, and then
  matrix multipy this with A^T. 

Parameters:
M	const long	cols of A, rows of A^T,y
N	const long	rows of A, cols of A^T,U,x
ALPHA	cosnt T		value to scale A^T with
A*	const T*	pointer to A
U*	const T*   	pointer to U
x*	const T*	pointer to x
BETA	const T		value to scale y with
y*	T*		pointer to y


-----------------------------------------------------------*/
#ifndef LINAL_ATUXPY_HPP
#define LINAL_ATUXPY_HPP

#include "simd.hpp"
#include <math.h>
#include "linal_def.hpp"

template <typename T>
void linal_ATUxpy(const long M, const long N, const T ALPHA, 
                  const T* A, const T* U, const T* x,
                  const T BETA, T* y);

/*
template <typename T, const int ALIGN>
void linal_ATUxpy(const long M, const long N, const T ALPHA, 
                  const T* A, const T* U, const T* x,
                  const T BETA, T* y);
*/

#endif
