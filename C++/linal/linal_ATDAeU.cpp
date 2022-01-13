/*--------------------------------------------------------
  linal_ATDAeU.cpp
 	JHT, January 1, 2022: created	

  .cpp file for the ATDAeU function, which solves the 
  following set of equations for the elements of the 
  diagonal matrix D (the vector D).
                        N             M
        N           ---------      -------          M
    ---------      | x       |    |       |      -----
   |         |     |   x     |    |       |     |_  U |
M  |   A^T   |   N |     x   |  N |   A   | = M   |_  |  
   |         | .   |       x | .  |       |         |_|
    ---------       ---------      -------

   Or (more clearly)

   A^T . D . A = U

   where A is an NxM matrix, D is an NxN diagonal matrix
   and U is an MxM upper symmetric matrix.

  NOTE: it is assumed that all the elements of A,D, and U
  are stored sequentially in memory, and only the diagonal 
  elements of D, and the upper triangular elments of U, 
  are stored

This proceeds in the following way:

K = M*(M+1)/2

1. The matrix Q(N,K) is constructed from the vectors
   of A, such that each column of Q corresponding to the 
   (i,j) matrix element of U is A(:,i) x A(:,j) (elementwise
   product)
2. The U matrix is copied over to the B vector
3. Then Q^T.X = B is solved for the N elements of X, corresponding
   to the diagonal elements of D to solve the original 
   equations. If N >= K, the solution is "exact". If N < K, the
   solution corresponds to a least squares to recreate U. 
4. If N < K, calculate the RMS error and max error of the least
   squares solution

Parameters
M	const long 	rows of U,A^T  | cols of D,A	
N	const long	cols of D,A^T  | rows of A
A	const double*	pointer to A matrix (NxM)
D	double* 	pointer to results vector (N) 
U	const double*	pointer to B upper triangular matrix (M*(M+1)/2)
WORK	double* 	pointer to work vector
LWORK	long		length of work vector
RMS	double		if N < M*(M+1)/2, the RMS error

WORK is long enough that neither A, nor U are destroyed
in this process
  
--------------------------------------------------------*/
#include "linal_ATDAeU.hpp"

//doubles only for now
void linal_ATDAeU(const long M, const long N, const double* A, 
                  double* D, const double* U, double& RMS,
                  double* WORK)
{
  const long K = (M*(M+1))/2;
  const long L = (K >= N) ? K : N;

/*1. The matrix Q(N,K) is constructed from the vectors
     of A, such that each column of Q corresponding to the 
     (i,j) matrix element of U is A(:,i) x A(:,j) (elementwise
     product)*/
  double* Q = WORK;
  double* WW = WORK + N*K;
  double* X = WW;
  WW += L;
  long ii=0;
  for (long j=0;j<M;j++)
  {
    for (long i=0;i<=j;i++)
    {
      simd_elemwise_mul<double>(N,A+N*i,A+N*j,Q+ii);
      ii+=N;
    }
  }
  printf("\nThe Q matrix is\n");
  linal_dgeprint(N,K,Q,N);
  
/* 2. The U matrix is copied over to the B vector */
  simd_copy<double>(K,U,X);

/* 3. Then Q^T.X = B is solved for the N elements of X, corresponding
      to the correct diagonal elements of D to solve the original 
      equations. */ 
  char TRANS = 'T';
  int NN = N;
  int KK = K;
  int NRHS = 1;
  int LWORK = -1;
  int LDX = L;
  int INFO;
  //get ideal length
  dgels_(&TRANS,&NN,&KK,&NRHS,Q,&NN,X,&LDX,WW,&LWORK,&INFO);
  LWORK = (long) *WW;
  WW += LWORK;
  //call it
  dgels_(&TRANS,&NN,&KK,&NRHS,Q,&NN,X,&LDX,WW,&LWORK,&INFO);

  //Copy the solution vector over to D
  simd_copy<double>(N,X,D);

  //Construct the rms and max error estimates
  if (N < K)
  {
    RMS = 0;
    for (long i=N;i<K;i++)
    {
      RMS += *(X) * *(X);
    }
    RMS = sqrt(RMS);
  } else {
    RMS = 0;
  }

  printf("\n RMS is %lf \n",RMS);
  printf("\n Solution vector is \n");
  linal_dgeprint(N,1,D,N);
   
}

long linal_ATDAeU_LWORK(const long M, const long N)
{

  const long K = (M*(M+1))/2;
  const long L = (K >= N) ? K : N;

  char TRANS = 'T';
  int MM = N;
  int NN = K;
  int NRHS = 1;
  double A;
  int LDA = N;
  double B;
  int LDB = L;
  double WORK;
  int LWORK = -1;
  int INFO;
  dgels_(&TRANS,&MM,&NN,&NRHS,&A,&LDA,&B,&LDB,&WORK,&LWORK,&INFO); 
  LWORK = (long) WORK;
  return LWORK + N*K + L;
} 

