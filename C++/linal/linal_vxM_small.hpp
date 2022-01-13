/*------------------------------------------------
  linal_vxM_small.hpp
    JHT, August 18, 2021

  - multiplies, element wise, a continuous vector
    with a continuous matrix, and stores the 
    result in a new matrix
  
------------------------------------------------*/
#ifndef LINAL_VXM_SMALL_HPP
#define LINAL_VXM_SMALL_HPP

template <typename T>
void linal_vxM_small(const int M, const int N, T* x, T* A, T* B); 

#endif
