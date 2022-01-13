/*----------------------------------------------------------
 simd.hpp
    JHT, October 22, 2021 : created

  .hpp file to help compilers vectorize commonly used 
  SIMD style functions. 

  NOTE : it is assumed the arrays are stored sequentially 
         in memory, for now

  NOTE : if targetting the YMM registers with something like
         AVX256 instructions, it is best to align your arrays
         to the appropriate BYTES, and use the specialized
         alignment functions 

  CURRENTLY SUPPORTED TPYES
  ------------------------------
  double,float,long,int

  CURRENTLY SUPPORTED OPERATIONS
  -------------------------------
  reductions    simd_reduction_opr<type[,alignment]>
  elementwise   simd_elemwise_opr<type[,alignment]>
  axpy          simd_axpy<type[,alignment]>
  dot           simd_dot<type[,alignment]>
  copy		simd_copy<type[,alignment]>
  zero		simd_zero<type[,alignment]>
  loc		simd_loc<type[alignment]>
  scalar op.    simd_scal_opr<type[,alignmet]>
  wxy		simd_wxz_opr<type[,alignment]>
  awxpy		simd_awxpy<type [,alignment]>
  raxmy		simd_raxmy<type[,alignment]>
  axpby         simd_axpby<type[,alignment]>

  COMMING SOON
  ---------------

----------------------------------------------------------*/

#ifndef SIMD_HPP
#define SIMD_HPP

#if defined(_OPENMP)
  #include <omp.h>
#endif

#if defined (__AVX2__) 
  #include <immintrin.h>
#endif

/*---------------------------------------------------------
 * reductions
 *
 * result = 0 (opr) X[0] (opr) X[1] (opr) ...
 *
 *   simd_reduction_opr<type [,align]>(const long N, const type* X)
 *   type   -> type of the data (int,long,float,double)
 *   align  -> optional, alignment of data in BYTES
 *   N      -> Number of elements to reduce
 *   X      -> address of first element to operate on 
 *
 *   currently supported operations (_opr):
 *   _add, _sub 
 *
 *   if provided, the alignment must be in BYTES, and, 
 *   if allocated dynamically, must be determined by the
 *   programmer or allocator. 
 *
 *   Note that XMM,YMM,ZMM need alignment of (16,32,and 64) 
 *   BYTES, respectively
 * -------------------------------------------------------*/
template <typename T>
T simd_reduction_add(const long N, const T* ptr);
template <typename T>
T simd_reduction_sub(const long N, const T* ptr);

template <typename T, const int ALIGNMENT>
T simd_reduction_add(const long N, const T* ptr);
template <typename T, const int ALIGNMENT>
T simd_reduction_sub(const long N, const T* ptr);

/*---------------------------------------------------------
 * element-wise 
 *
 *  Operations on two pointers, storing the result in a 
 *  third pointer: Z = X (opr) Y
 *
 *   simd_elemwise_opr<type [,align]>(const long N, const type* X,
 *                                    const type* Y, type* Z)
 *   type   -> type of the data (int,long,float,double)
 *   align  -> optional, alignment of data in BYTES of all three pointers
 *   N      -> Number of elements to reduce
 *   X      -> address of first element of X to operate on 
 *   Y      -> address of first element of Y to operator on
 *   Z      -> address of first element of Z to store results
 *
 *   Currently supported operations (_opr):
 *   _add, _mul 
 *
 * -------------------------------------------------------*/
template <typename T>
void simd_elemwise_add(const long N, const T* X, const T* Y, T* Z);
template <typename T>
void simd_elemwise_mul(const long N, const T* X, const T* Y, T* Z);

template <typename T, const int ALIGNMENT>
void simd_elemwise_add(const long N, const T* X, const T* Y, T* Z);
template <typename T, const int ALIGNMENT>
void simd_elemwise_mul(const long N, const T* X, const T* Y, T* Z);

/*---------------------------------------------------------
 * axpy
 *
 * Scales the values of pointer X by const A 
 *   and adds them to pointer Y
 *
 *  simd_axpy<type [,align]>(const long N, const type A, const type* X, type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on	
 *  A      -> scalar to multiply X
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/
template <typename T>
void simd_axpy(const long N, const T A, const T* X, T* Y);

template <typename T, const int ALIGNMENT>
void simd_axpy(const long N, const T A, const T* X, T* Y);

/*---------------------------------------------------------
 * dot 
 *
 * Returns the dot product of pointer X and pointer Y 
 *
 *  simd_dot<type [,align]>(const long N, const type* X, const type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on	
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/
template <typename T>
T simd_dot(const long N, const T* X, const T* Y);

template <typename T, const int ALIGNMENT>
T simd_dot(const long N, const T* X, const T* Y);

/*---------------------------------------------------------
 * scal
 * scales the values of sequential memory
 *
 *  simd_scal<type>(const long N, const type A, type* X)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES
 *  N      -> long, Number of elements to act on
 *  A      -> type, scalar factor
 *  X*     -> address of first element of X to act on 
 * -------------------------------------------------------*/

/*---------------------------------------------------------
 * copy
 * copies the values in memory from one location to another 
 *
 *  simd_copy<type>(const long N, const type* X, type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES
 *  N      -> long, Number of elements to act on
 *  X*     -> address of first element of X to act on 
 *  Y*     -> address of first element of Y to act on 
 * -------------------------------------------------------*/
template <typename T>
void simd_copy(const long N, const T* X, T* Y);

template <typename T, const int ALIGNMENT>
void simd_copy(const long N, const T* X, T* Y);

/*---------------------------------------------------------
 * zero
 * zeros the values of sequential memory
 *
 *  simd_zero<type[,align]>(const long N, type* X)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES
 *  N      -> long, Number of elements to act on
 *  X*     -> address of first element of X to act on 
 * -------------------------------------------------------*/
template <typename T>
void simd_zero(const long N, T* X);

template <typename T, const int ALIGNMENT>
void simd_zero(const long N, T* X);

/*---------------------------------------------------------
 * loc 
 *  finds the first entry of a value in array X. If no entry
 *  matches, returns -1 
 *
 *  NOTE: this currently only works with int,long types
 *
 *  simd_loc<type[,align]>(const long N, const type A, const type* X)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES
 *  N      -> long, Number of elements to act on
 *  A      -> value to match 
 *  X*     -> address of first element of X to act on 
 * -------------------------------------------------------*/
template <typename T>
long simd_loc(const long N, const T A, const T* X);

template <typename T, const int ALIGNMENT>
long simd_loc(const long N, const T A, const T* X);

/*---------------------------------------------------------
 * scal_opr
 *   performs a scalar operation of value A on array X. 
 *
 * simd_scal_opr<type[,align]>(const long N, const type A, type* X)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES
 *  N      -> long, number of elements to act on
 *  A      -> value to perform scalar operation with
 *  X*     -> pointer to first element of X
 *
 *   Currently supported operations (_opr):
 *   _add,_mul,_set
 *
 * -------------------------------------------------------*/
template <typename T>
void simd_scal_add(const long N, const T A, T* X);
template <typename T>
void simd_scal_mul(const long N, const T A, T* X);
template <typename T>
void simd_scal_set(const long N, const T A, T* X);

template <typename T, const int ALIGNMENT>
void simd_scal_add(const long N, const T A, T* X);
template <typename T, const int ALIGNMENT>
void simd_scal_mul(const long N, const T A, T* X);
template <typename T, const int ALIGNMENT>
void simd_scal_set(const long N, const T A, T* X);


/*---------------------------------------------------------
 * wxy 
 *
 *  Operations on three pointers, storing the result in a 
 *  third pointer: Z = W (OPR) X (OPR) Y  
 *
 *   simd_wzy_opr<type [,align]>(const long N, const type* W, 
 *                               const type* X,const type* Y,
 *				 type* Z)
 *   type   -> type of the data (int,long,float,double)
 *   align  -> optional, alignment of data in BYTES of all three pointers
 *   N      -> Number of elements to reduce
 *   W      -> address of first element of W to operate on
 *   X      -> address of first element of X to operate on 
 *   Y      -> address of first element of Y to operator on
 *   Z      -> address of first element of Z to store results
 *
 *   Currently supported operations (_opr):
 *   _mul 
 *
 * -------------------------------------------------------*/
template <typename T>
void simd_wxy_mul(const long N, const T* W, const T* X, const T* Y, T* Z);

template <typename T, const int ALIGNMENT>
void simd_wxy_mul(const long N, const T* W, const T* X, const T* Y, T* Z);

/*---------------------------------------------------------
 * dotwxy 
 *
 * Returns the dot product of pointer W and pointer Z and pointer Y 
 *
 *  simd_dotwxy<type [,align]>(const long N, const type* W, const type* X, const type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on	
 *  W*     -> Address of first element of W to act on
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/
template <typename T>
T simd_dotwxy(const long N, const T* W, const T* X, const T* Y);

template <typename T, const int ALIGNMENT>
T simd_dotwxy(const long N, const T* W, const T* X, const T* Y);

/*---------------------------------------------------------
 * awxpy 
 *
 * element wise multiplies vectors w and x, scales the product by 
 * constant a, and adds the result to a vector y 
 *
 *  simd_awxpy<type [,align]>(const long N, const type a, const type* W, const type* X, type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on	
 *  A      -> scalar to multiply w*x by 
 *  W*     -> Address of first element of W to act on
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/
template <typename T>
void simd_awxpy(const long N, const T A, const T* W, const T* X, T* Y);

template <typename T,const int ALIGNMENT>
void simd_awxpy(const long N, const T A, const T* W, const T* X, T* Y);

/*---------------------------------------------------------
 * raxmy
 *
 * take the reciprocal of vector x, scales it by constant a
 * and multiplies the result into vector y 
 * 
 *
 *  simd_raxmy<type [,align]>(const long N, const type a, const type* X, type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on	
 *  A      -> scalar to multiply w*x by 
 *  X*     -> Address of first element of X to act on
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/
template <typename T>
void simd_raxmy(const long N, const T A, const T* X, T* Y);

template <typename T,const int ALIGNMENT>
void simd_raxmy(const long N, const T A, const T* X, T* Y);

/*---------------------------------------------------------
 * axpby
 *
 * Scales the values of pointer X by const A, and adds them
 * to a pointer Y, whose values are scaled by const B 
 *
 *  simd_axpby<type [,align]>(const long N, const type A, const type* X, 
                              const type B, type* Y)
 *
 *  type   -> type of the data (int, long, float, double)
 *  align  -> optional, int, alignment of data in BYTES of all pointers
 *  N      -> number of elements to act on	
 *  A      -> scalar to multiply X
 *  X*     -> Address of first element of X to act on
 *  B      -> scalar to multiply Y
 *  Y*     -> Address of first element of Y to act on
 * -------------------------------------------------------*/
template <typename T>
void simd_axpby(const long N, const T A, const T* X, const T B, T* Y);

template <typename T, const int ALIGNMENT>
void simd_axpby(const long N, const T A, const T* X, const T B, T* Y);

#endif
