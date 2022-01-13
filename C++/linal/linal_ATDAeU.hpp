/*--------------------------------------------------------
  linal_ATDAeU.hpp
 	JHT, January 1, 2022: created	

  .hpp file for the ATDAeU function, which solves the 
  following set of equations for the elements of the 
  diagonal matrix D (the vector D).
                        N             M
        N           ---------      -------          M
    ---------      | x       |    |       |      -----
   |         |     |   x     |    |       |     |_  U |
M  |   A^T   |   N |     x   |  N |   A   | = M   |_  |  
   |         | .   |       x | .  |       |         |_|
    ---------       ---------      -------

   Or (more clearly)

   A^T . D . A = U

   where A is an NxM matrix, D is an NxN diagonal matrix
   and Y is an MxM upper symmetric matrix.

  NOTE: it is assumed that all the elements of A,D, and U
  are stored sequentially in memory, and only the diagonal 
  elements of D, and the upper triangular elments of U, 
  are stored

Parameters
M	const long 	rows of U,A^T  | cols of D,A	
N	const long	cols of D,A^T  | rows of A
A	const double*	pointer to A matrix (NxM)
D	double* 	pointer to results vector (N) 
U	const double*	pointer to B upper triangular matrix (M*(M+1)/2)
RMS	double&		RMS of solution vector
WORK	double* 	pointer to work vector
LWORK	long		length of work vector

WORK is long enough that neither A,B, nor X are destroyed
in this process
  
--------------------------------------------------------*/
#ifndef LINAL_ATDAeU_HPP
#define LINAL_ATDAeU_HPP

#include "lapack_interface.hpp"
#include "linal_geprint.hpp"
#include "simd.hpp"
#include <math.h>

//doubles only for now
void linal_ATDAeU(const long M, const long N, const double* A, 
                  double* D, const double* U, double& RMS,
                  double* WORK);

long linal_ATDAeU_LWORK(const long M, const long N); 

#endif
