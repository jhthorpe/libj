/*---------------------------------------------------------------
  lapack_interface.hpp
    JHT July 17, 2021	: created

    - implementation of interfaces with F77 LAPACK 

---------------------------------------------------------------*/
#ifndef LAPACK_INTERFACE_HPP
#define LAPACK_INTERFACE_HPP

#ifdef __cplusplus
extern "C" {
#endif

  extern void dsygv_(int* ITYPE, char* JOBZ, char* UPLO, int* N, 
                     double* A, int* LDA,
                     double* B, int* LDB,
                     double* W, double* WORK, int* LWORK, int* INFO);

  extern void dsysv_(char* UPLO, int* N, int* NRHS, double* A, 
                     int* LDA, int* IPIV, double* B, int* LDB,
                     double* WORK, int* LWORK, int* INFO);

  extern void dgels_(char* TRANS, int* M, int* N, int* NHRS,
                     double* A, int* LDA, double* B, int* LDB,
                     double* WORK, int* LWORK, int* INFO);

  extern void dgesv_(int* N, int* NRHS, double* A, int* LDA, 
                     int* IPIV, double* B, int* LDB, int* INFO); 
 
  extern void dgesvd_(char* JOBU, char* JOBVT, int* M, int* N,
                      double* A, int* LDA, double* S, double* U, 
                      int* LDU, double* VT, int* LDVT, double* WORK,
                      int* LWORK, int* INFO);


#ifdef __cplusplus
}
#endif

#endif

