/*-------------------------------------------------------------
  linal_diag_ABpC.hpp
	JHT, December 30, 2021 : created

  .hpp file for linal_diag_ABpC, which calcules only the 
  M diagonal elements resulting from the following matrix
  operation:

  diag C = diag ( alpha*A*B + beta*C )

  NOTE : it is assumed all matrices and vectors are 
         stored sequentially in memory 


Parameters
M	long		rows
N	long		cols
ALPHA	const T		constant to multiply A	
A	const double*	A (MxN)
B	const double*   B (NxM)
BETA	const T		constant to multiply C
C	T*		C (M) diagonal elements
-------------------------------------------------------------*/
#ifndef LINAL_DIAG_ABPC_HPP
#define LINAL_DIAG_ABPC_HPP

#include "linal_def.hpp"
#include <math.h>

template<typename T>
void linal_diag_ABpC(const long M, const long N, const T ALPHA,
                     const T* A, const T* B, const T BETA, T* C);

#endif
