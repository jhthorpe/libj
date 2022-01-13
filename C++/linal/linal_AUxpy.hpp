/*-----------------------------------------------------------
  linal_AUxpy.cpp
	JHT, January 7, 2022 : created

  .cpp file for AUxpy, which matrix multiplies a
  MxN matrix A (which may be scaled), 
  and NxN upper symmetric matrix U, a N vector x, and adds
  them to a (scaled) M vector y  **IN PLACE** 

  It is assumed that A,U,v, and y are held continuously 
  in memory, that A is stored column major, and that
  U is 
  
           A            U         x                y
         _______    ________     ___              ___
        | 0 3 5 |   \ 0 1 3 |   | 0 |            | 0 |
alpha*  | . . . |     \ . . |   | . |  += beta * | . |
        | . . . |       \ . |   | . |            | . |
         -------  .      ---  .  ---              ---

Parameters:
M	const long	rows of A,y
N	const long	cols of A,U,x
ALPHA	cosnt T		value to scale A^T with
A*	const T*	pointer to A
U*	const T*   	pointer to U
x*	const T*	pointer to x
BETA	const T		value to scale y with
y*	T*		pointer to y


-----------------------------------------------------------*/
#ifndef LINAL_AUXPY_HPP
#define LINAL_AUXPY_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template <typename T>
void linal_AUxpy(const long M, const long N, const T ALPHA, 
                  const T* A, const T* U, const T* x,
                  const T BETA, T* y);

/*
template <typename T, const int ALIGN>
void linal_AUxpy(const long M, const long N, const T ALPHA, 
                  const T* A, const T* U, const T* x,
                  const T BETA, T* y);
*/

#endif
