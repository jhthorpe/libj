/*------------------------------------------------
  linal_MTM_UP_small.hpp
    JHT, August 18, 2021

  - multiplies, element wise, a two continuous
    vectors and stores the result in a new vector
  
------------------------------------------------*/
#ifndef LINAL_MTM_UP_SMALL_HPP
#define LINAL_MTM_UP_SMALL_HPP

template <typename T>
void linal_MTM_UP_small(const int M, const int N,const int K,const T ALPHA, T* A, T* B,const T BETA, T* C);

#endif
