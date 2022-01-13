/*------------------------------------------------
  linal_ATBpC.hpp
        JHT, December 8, 2021 : created 

    C = ALPHA*A^T.B + BETA*C 

    It is assumed that C,A,and B are all 
    continous in memory and coloumn major. 
    Logical dimension == physical dimension 
------------------------------------------------*/
#ifndef LINAL_ATBPC_HPP
#define LINAL_ATBPC_HPP

#include "simd.hpp"
#include "linal_def.hpp"
#include <math.h>

template <typename T>
void linal_ATBpC(const int M, const int N, const int K,  
                 const T ALPHA, T* A, T* B, const T BETA, 
                 T* C);

/*
template <typename T, const int ALIGN>
void linal_ATBpC(const int M, const int N, const int K,  
                 const T ALPHA, T* A, T* B, const T BETA, 
                 T* C);
*/
#endif
