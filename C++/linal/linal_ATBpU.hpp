/*------------------------------------------------
  linal_ATBpU_UP
        JHT, December 8, 2021 : created 
	JHT, January 2, 2022 : renamed

    C = alpha*A^T.B + beta*C

    Only computes the upper triagular results of C. It is assumed that 
    A,B, and C are all continuous in memory, and stored column major

------------------------------------------------*/
#ifndef LINAL_ATBpU_HPP
#define LINAL_ATBpU_HPP

#include "simd.hpp"

template <typename T>
void linal_ATBpU(const int M, const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C);

/*
template <typename T, const int ALIGN>
void linal_ATBpU(const int M, const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C);
*/
#endif
