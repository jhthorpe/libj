/*--------------------------------------------------------
  linal_geprint.hpp
	JHT, December 30, 2021 : created

  .hpp file for printing general matrices that are 
  stored column major
--------------------------------------------------------*/
#ifndef LINAL_GEPRINT_HPP
#define LINAL_GEPRINT_HPP
#include <stdio.h>
void linal_dgeprint(const long M, const long N, const double* A, const long LDA);
#endif
