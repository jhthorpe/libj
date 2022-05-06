/*----------------------------------------------------------------------------------
  jblis_L1.hpp
	JHT, April 29, 2022 : created

  .hpp file for the C++ interface with jblis, my (bad) implementation of tblis

  L1 defines the level-1 implementations, which includes the following routines:

    zero
    set
    scale
    copy

----------------------------------------------------------------------------------*/
#ifndef JBLIS_L1_HPP
#define JBLIS_L1_HPP

#include <algorithm>
#include "tensor.hpp"
#include "tensor_matrix.hpp"
#include "scatter_matrix.hpp"
#include "block_scatter_matrix.hpp"
#include "libjdef.h"

#if defined (__AVX512F__)
  #include <immintrin.h>
  #define LIBJ_AVX512F 1
#endif

#if defined (__FMA__)
  #include <immintrin.h>
  #define LIBJ_FMA 1
#endif

#if defined (__AVX2__)
  #include <immintrin.h>
  #define LIBJ_AVX2 1
#endif

#if defined (__AVX__)
  #include <immintrin.h>
  #define LIBJ_AVX 1
#endif

#if defined (LIBJ_OMP)
  #include <omp.h>
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
 * Copy a scaled tensor X to tensor Y. Special cases of 
 * a == 1 and a == 0 are coded 
 *
 * a	-> scalar for X
 * X	-> tensor to copy from
 * Y	-> tensor to copy to
---------------------------------------------------------*/
template <typename T>
void scopy(const T a, const libj::tensor<T>& X, libj::tensor<T>& Y); 
template <typename T>
void copy(const libj::tensor<T>& X, libj::tensor<T>& Y);

}//end libj 
#endif
