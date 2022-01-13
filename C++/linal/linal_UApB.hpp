/*-------------------------------------------------------------------------
  linal_UApB.hpp
	JHT, January 11, 2022 : created

  .hpp file for UApB, which multiplies a scaled, upper symmetric matrix
  U of size MxM by matrix A of size MxN, and adds the result to a 
  scaled matrix B of size MxN

  It is assumes that U,A, and B are stored continously in memory, and
  that only the upper symmetric parts of U are stored

     B = (a*U).B + (b*B) 
                  M                N                      N
   (           _______   )       _____        (         _____  )
  (           \ 0 1 3 |   )     | 0 3 |      (         | 0 3 |  )
 (  alpha* M    \ . . |    )  M | . . |  +  ( beta*  M | . . |   )
  (               \ . |   )     | . . |      (         | . . |  )
   (                --  )    x   -----        (         -----  )

Parameters
name         type           size         description
M	     const long     1            rows of U,A,B
N            const long     1            cols of A,B
ALPHA        const T        1            constant to scale U by
U            const T*       (M*(M+1))/2  pointer to matrix U    
A            const T*       M*N          pointer to matrix A
BETA         const T        1            constant to scale B by
B            T*             M*N          pointer to matrix B
-------------------------------------------------------------------------*/
#ifndef LINAL_UAPB_HPP
#define LINAL_UAPB_HPP

#include "linal_def.hpp"
#include "simd.hpp"
#include <math.h>

template <typename T>
void linal_UApB(const long M, const long N, const T ALPHA, const T* U,
                const T* A, const T BETA, T* B);

/*
template <typename T, const int ALIGN>
void linal_UApB(const long M, const long N, const T ALPHA, const T* U,
                const T* A, const T BETA, T* B);
*/
#endif
