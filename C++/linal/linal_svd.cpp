/*-------------------------------------------------
  linal_svd.hpp
	JHT, December 30, 2021 : created

  .hpp file that handles construction of the 
  SVD decomposition of matrix A :

  A = U . S . V^T

  A is MxN
  U is MxM
  S is MxN
  V is NxN

  U and V are the left and right singular 
  vectors (of which there are min(M,N))

  the min(M,N) diagonal elements of S are the 
  singular values, the rest are zero
  Currently, this is handled as a call to LAPACK

Parameters
M       long    #rows of A
N       long    #cols of A
A       T*      matrix to SVD (MxN)
S       T*      vector of sing. values min(MxN)
U       T*      RHS sing. vectors (MxM)
VT      T*      LHS sing. vectors (NxN)
WORK    T*      working vector (LWORK)
LWORK   long&   length of working vector
INFO    int&    job status

-------------------------------------------------*/
#include "lapack_interface.hpp"

//Double code
void linal_dsvd(const long M, const long N, double* A, double* S, double* U, 
               double* VT, double* WORK, long& LWORK, int& INFO)
{
  char JOBU  = 'A';
  char JOBVT = 'A';
  int  MM    = M;
  int  NN    = N;
  int LDA    = MM;
  int LDU    = MM;
  int LDVT   = NN;
  int LWORKK = LWORK;

  dgesvd_(&JOBU,&JOBVT,&MM,&NN,A,&LDA,S,U,&LDU,VT,&LDVT,WORK,&LWORKK,&INFO); 

}

long linal_dsvd_LWORK(const long M, const long N)
{
  char JOBU  = 'A';
  char JOBVT = 'A';
  int  MM    = M;
  int  NN    = N;
  int LDA    = MM;
  int LDU    = MM;
  int LDVT   = NN;
  int LWORK  = -1;
  int INFO;
  double D;
  
  dgesvd_(&JOBU,&JOBVT,&MM,&NN,&D,&LDA,&D,&D,&LDU,&D,&LDVT,&D,&LWORK,&INFO); 

  return (long) D;

}

/*
template<>
void linal_svd<T>(const long M, const long N, T* A, T* S, T* U, 
                  T* VT, T* WORK, long& LWORK, int& INFO)
{
  char JOBU  = 'A';
  char JOBVT = 'A';
  int  MM    = M;
  int  NN    = N;
  int LDA    = MM;
  int LDU    = MM;
  int LDV    = NN;
  int LWORKK = LWORK;

  dgesvd_(&JOBU,&JOBVT,&MM,&NN,A,&LDA,S,U,&LDU,VT,&LDVT,WORK,&LWORKK,&INFO); 

  if (LWORK == -1)
  {
    LWORK = (long) *WORK;
  }

}
*/
