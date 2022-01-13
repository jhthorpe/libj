/*---------------------------------------------------------------
  blas_interface.hpp
    JHT July 17, 2021 : created

    - implementation of interfaces with F77 BLAS
---------------------------------------------------------------*/
#ifndef BLAS_INTERFACE_HPP
#define BLAS_INTERFACE_HPP

#ifdef __cplusplus
extern "C" {
#endif 
  //LEVEL 1
  extern double ddot_(int* N, double* X, int* INCX, double* Y, int* INCY);
  extern double dnrm2_(int* N, double* X, int* INCX);
  extern void daxpy_(int* N, double* alpha, double* X, 
                     int* INCX, double* Y, int* INCY);
  extern void dscal_(int* N, double* DA, double* DX, int* INCX);

  //LEVEL 2
  extern void dgemv_(char* trans, int* M, int* N, double* alpha,
                       double* A, int* LDA,
                       double* X, int* INCX, double* BETA,
                       double* Y, int* INCY);

  //LEVEL 3
  extern void dgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K,
                     double* ALPHA, double* A, int* LDA,
                     double* B,int* LDB,
                     double* BETA, double* C, int* LDC);
  extern void dsymm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K,
                     double* ALPHA, double* A, int* LDA,
                     double* B,int* LDB,
                     double* BETA, double* C, int* LDC);

#ifdef __cplusplus
}
#endif

#endif

