/*--------------------------------------------------------
  linal_geprint.cpp
	JHT, December 30, 2021 : created

  .cpp file for printing general matrices that are 
  stored column major
--------------------------------------------------------*/
#include "linal_geprint.hpp"
void linal_dgeprint(const long M, const long N, const double* A, const long LDA)
{
  for (long i=0;i<M;i++) 
  {
    for (long j=0;j<N;j++)
    {
      printf("%18.15E  ",*(A+LDA*j));
    } 
    printf("\n");
    A++;
  }
}

