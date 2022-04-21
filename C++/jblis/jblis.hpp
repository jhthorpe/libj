/*----------------------------------------------------------------------------------
  jblis.hpp
	JHT, April 11, 2022 : created

  .hpp file for the C++ interface with jblis, my (bad) implementation of tblis
----------------------------------------------------------------------------------*/
#ifndef JBLIS_HPP
#define JBLIS_HPP

#include <algorithm>
#include "tensor.hpp"

#if defined (__AVX512F__)
  #include <immintrin.h>
  #define AVX512F 1
#endif

#if defined (__FMA__)
  #include <immintrin.h>
  #define FMA 1
#endif

#if defined (__AVX2__)
  #include <immintrin.h>
  #define AVX2 1
#endif

#if defined (__AVX__)
  #include <immintrin.h>
  #define AVX 1
#endif

#if defined (_OPENMP)
  #include <omp.h>
  #define OMP 1
#endif

namespace libj
{

/*---------------------------------------------------------
 * zero
 * 
 *  Set tensor to zero
 * 
 * A 	-> tensor to zero  
---------------------------------------------------------*/
template <typename T>
void zero(libj::tensor<T>& A);

/*---------------------------------------------------------
 * set 
 * 
 *  Set tensor to to a constant value
 * 
 * scal -> value to set with
 * A 	-> tensor 
---------------------------------------------------------*/
template <typename T>
void set(const T scal,libj::tensor<T>& A);

/*---------------------------------------------------------
 * scal
 *
 * Scale tensor by constant value
 *
 * scal -> value to scale with
 * A    -> tensor  
---------------------------------------------------------*/
template <typename T>
void scal(const T s, libj::tensor<T>& A);

/*---------------------------------------------------------
 * Copy functions 
 *
 * Scopy:
 * Copy a scaled tensor X to tensor Y. Special cases of 
 * a == 1 and a == 0 are coded 
 *
 * Copy:
 * Copy an unscaled tensor X to tensor Y
 *
 * a	-> scalar for X
 * X	-> tensor to copy from
 * Y	-> tensor to copy to
---------------------------------------------------------*/
template <typename T>
void scopy(const T a, const libj::tensor<T>& X, libj::tensor<T>& Y); 
template <typename T>
void copy(const libj::tensor<T>& X, libj::tensor<T>& Y);

/*---------------------------------------------------------
 * axpby functions 
 * Y = a*X + b*Y
 *
 * Adds tensor X to tensor Y, where X is scaled by a 
 * and Y is scaled by b. Different code for the 
 * special cases is indicated by lack of a or b. 
 * 
 * calling axpby will always call the correct simplification
 *
 * a	-> scalar for X
 * X	-> tensor 
 * b	-> scalar for Y
 * Y	-> tensor
---------------------------------------------------------*/
template <typename T>
void axpby(const T a, const libj::tensor<T>& X, const T b, libj::tensor<T>& Y);
template <typename T>
void xpby(const libj::tensor<T>& X, const T b, libj::tensor<T>& Y);
template <typename T>
void axpy(const T a, const libj::tensor<T>& X, libj::tensor<T>& Y);
template <typename T>
void xpy(const libj::tensor<T>& X, libj::tensor<T>& Y);

/*---------------------------------------------------------
  * reduction functions
  *
  * Accumulates all the elements of tensor X
  *
  * X	-> tensor
---------------------------------------------------------*/
template <typename T>
T sum(const libj::tensor<T>& X);

}//end libj 
#endif
