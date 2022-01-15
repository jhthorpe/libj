/* blis.h
	JHT, January 14, 2021 : created

  .hpp file for JBLIS, which uses the BLIS strategy for constructing linear algebra routines. 

  Two versions of each routine will be provided. One which runs in serial, and one which
  runs in parallel. The parallel code is marked with a _p at the end
  
  For the moment, only column-major operations are supported, and only on the 
  int,float,long, and double types. Currently, only the double type gets special, performance 
  oriented code.  

  See.
Field G. Van Zee and Robert A. van de Geijn. 2015. BLIS: A framework for rapidly instanting BLAS func- tionality. ACM Trans. Math. Softw. 41, 3, Article 14 (May 2015), 33 pages.
DOI: http://dx.doi.org/10.1145/2764454

*/

#ifndef JBLIS_H
#define JBLIS_H

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

#ifdef __cplusplus
extern "C" {
#endif



/* 
	Level 1v operations 
  These operations are performed on vectors. 

  Here, a,b,c indicate scalars, x,y indicate constants,m ^T indicates a transpose

  CURRENTLY SUPPORTED


  COMMING SOON
  zerov		x = 0			Set values of x to zero
  setv		x = a			set values of x to const
  scalv		x = a*x			scale values of x by const
  copyv		y = x 			copy values of x to y
  scal2v	y = a*x			scale values of x and copy into y
  axpyv		y = y + a*x		add scaled elements of x to y 
  dotv		b = x^T . y		dot product of x and y
  dotxy		b = c*b + a*x^T.y	scale b, scale result of dot x.y
  invertv	y = y^-1		elementwise invert

  //JHT special code
  raxmyv	y = y * a*x^-1		scale x reciprocal elements, multiply y elementwise
  axpbyv	y = b*y + a*x		scale x by a, y by b, add and store in y
  abslocv	i = (x_i == a)		returns the first location in x that matches a to a tol 
  absminv	i = min(|x_i|)		returns the min absolute value in vector 
  absmaxv	i = MAX(|x_i|)		returns the max absolute value in vector 
*/

/*-----------------------------------------------------
  zerov
	Zeros the elements of a vector X

	JHT, January 15, 2022 : created

Paramters
name       type          size        description
N          const long  	 1	     number of elements
X          double*       N           elements to zero


Supported Types
double		
-----------------------------------------------------*/
void libj_dzerov(const long N, double* X);
void libj_dzerov_p(const long N, double* X);


#ifdef __cplusplus
}
#endif

#endif
