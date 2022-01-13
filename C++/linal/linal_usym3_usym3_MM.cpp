/*----------------------------------------------------------------
  linal_usym3_usym3_MM.cpp
	JHT, September 2, 2021

  - matrix multiplies an upper symmetric (triangular) matrix A
    by upper symmetric (triangular) matrix B, stored in the 
    general matrix C. This is the special case where all matrices
    are 3x3, and stored...

       C      =      A      .      B
  | 0  3  6 |   | 0  1  3 |   | 0  1  3 |
  | 1  4  7 | = |    2  4 |   |    2  4 |
  | 2  5  8 |   |       5 | . |       5 |

      
-----------------------------------------------------------------*/
#include "linal_usym3_usym3_MM.hpp"
template <typename T>
 void linal_usym3_usym3_MM(const T* A, const T* B, T* C)
{
  *(C+0) = *(A+0)**(B+0) + *(A+1)**(B+1) + *(A+3)**(B+3);
  *(C+1) = *(A+1)**(B+0) + *(A+2)**(B+1) + *(A+4)**(B+3);
  *(C+2) = *(A+3)**(B+0) + *(A+4)**(B+1) + *(A+5)**(B+3);
  *(C+3) = *(A+0)**(B+1) + *(A+1)**(B+2) + *(A+3)**(B+4);
  *(C+4) = *(A+1)**(B+1) + *(A+2)**(B+2) + *(A+4)**(B+4);
  *(C+5) = *(A+3)**(B+1) + *(A+4)**(B+2) + *(A+5)**(B+4);
  *(C+6) = *(A+0)**(B+3) + *(A+1)**(B+4) + *(A+3)**(B+5);
  *(C+7) = *(A+1)**(B+3) + *(A+2)**(B+4) + *(A+4)**(B+5);
  *(C+8) = *(A+3)**(B+3) + *(A+4)**(B+4) + *(A+5)**(B+5);
}

template  void linal_usym3_usym3_MM<double>(const double* A, const double* B, double* C);
template  void linal_usym3_usym3_MM<float>(const float* A, const float* B, float* C);
template  void linal_usym3_usym3_MM<int>(const int* A, const int* B, int* C);
template  void linal_usym3_usym3_MM<long>(const long* A,const long* B, long* C);
