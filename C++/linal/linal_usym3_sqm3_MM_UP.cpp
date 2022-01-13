/*----------------------------------------------------------------
  linal_usym3_sqm3_MM_UP.cpp
	JHT, September 2, 2021

  - matrix multiplies an upper symmetric (triangular) matrix A
    by square, general (triangular) matrix B, stored in the 
    general matrix C. This is the special case where all matrices
    are 3x3, and stored...

        C      =      A      .      B
   | 0  1  3 |   | 0  1  3 |   | 0  3  6 |
   |    2  4 | = |    2  4 |   | 1  4  7 |
   |       5 |   |       5 | . | 2  5  8 |


      
-----------------------------------------------------------------*/
#include "linal_usym3_sqm3_MM_UP.hpp"
template <typename T>
 void linal_usym3_sqm3_MM_UP(const T* A, const T* B, T* C)
{
  *(C+0) = *(A+0)**(B+0) + *(A+1)**(B+1)+ *(A+3)**(B+2);
  *(C+1) = *(A+0)**(B+3) + *(A+1)**(B+4)+ *(A+3)**(B+5);
  *(C+2) = *(A+1)**(B+3) + *(A+2)**(B+4)+ *(A+4)**(B+5);
  *(C+3) = *(A+0)**(B+6) + *(A+1)**(B+7)+ *(A+3)**(B+8);
  *(C+4) = *(A+1)**(B+7) + *(A+2)**(B+7)+ *(A+4)**(B+8);
  *(C+5) = *(A+3)**(B+7) + *(A+4)**(B+7)+ *(A+5)**(B+8);
}

template  void linal_usym3_sqm3_MM_UP<double>(const double* A, const double* B, double* C);
template  void linal_usym3_sqm3_MM_UP<float>(const float* A, const float* B, float* C);
template  void linal_usym3_sqm3_MM_UP<int>(const int* A, const int* B, int* C);
template  void linal_usym3_sqm3_MM_UP<long>(const long* A,const long* B, long* C);
