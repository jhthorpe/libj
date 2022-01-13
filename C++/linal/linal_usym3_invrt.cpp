/*----------------------------------------------------------------
  linal_usym3_invrt.cpp
	JHT, September 1, 2021

  Inverts an symmetric 3x3 matrix, which is assumed to be
  stored upper triangular.
-----------------------------------------------------------------*/
#include "linal_usym3_invrt.hpp"
template <typename T>
 void linal_usym3_invrt(T* A)
{
  const T A0 = *(A+2)**(A+5) - *(A+4)**(A+4);
  const T A1 = *(A+3)**(A+4) - *(A+1)**(A+5);
  const T A2 = *(A+0)**(A+5) - *(A+3)**(A+3);
  const T A3 = *(A+1)**(A+4) - *(A+2)**(A+3);
  const T A4 = *(A+1)**(A+3) - *(A+0)**(A+4);
  const T A5 = *(A+0)**(A+2) - *(A+1)**(A+1);
  //Yes, the plus is correct. A1 is the minus of the det needed
  const T ADET = (T)1/(*(A+0)*A0 + *(A+1)*A1 + *(A+3)*A3);
  *(A+0) = ADET*A0;
  *(A+1) = ADET*A1;
  *(A+2) = ADET*A2;
  *(A+3) = ADET*A3;
  *(A+4) = ADET*A4;
  *(A+5) = ADET*A5;
}

template  void linal_usym3_invrt<double>(double* A);
template  void linal_usym3_invrt<float>(float* A);
template  void linal_usym3_invrt<int>(int* A);
template  void linal_usym3_invrt<long>(long* A);
