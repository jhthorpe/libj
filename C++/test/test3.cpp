
#include "libj.hpp"

int main()
{
  const int NABS=3;
  const int NBAS=2;
  gemat<double>  A(NBAS,NBAS);
  gemat<double>  X(NABS,NBAS);
  gemat<double>  Y(NABS,NBAS);
  gemat<double>  XTMP(NABS,NBAS);
  vec<double>    V(NABS);
  vec<double>    V2(NABS);
  vec<double>    V3(NABS);
  int LWORK = 1000;
  vec<double>    WORK(LWORK);
  vec<int>       IPIV(NBAS);

  V(0) = 0;
  V(1) = 1;
  V(2) = 2; 
  
  X(0,0) = 1;
  X(1,0) = 1;
  X(2,0) = 1;
  X(0,1) = 2;
  X(1,1) = 4;
  X(2,1) = 6;

  //Form the test matrix A = X^T.VxX

  //scalar multiply VxX into Y
  linal_DApB<double>(NABS,NBAS,1,&V[0],&X[0],0,&Y[0]);

  //matrix multiply X and Y
  char TRANSA = 'T';
  char TRANSB = 'N';
  int M = NBAS;
  int N = NBAS;
  int K = NABS;
  double alpha = 1;
  int LDA = NABS;
  int LDB = NABS;
  double beta = 0;
  int LDC = NBAS;

  printf("V vector \n");
  V.print();
  printf("\n X matrix \n");
  X.print();
  printf("\n Y matrix \n");
  Y.print();
  
  dgemm_(&TRANSA,&TRANSB,&M,&N,&K,&alpha,&X[0],&LDA,&Y[0],&LDB,
         &beta,&A[0],&LDC); 

  printf("\n A matrix \n");
  A.print(); 

  
  printf("\n \n");
  printf("Now, we want to see if we can recreate V \n");

/*
  //solve X^T.Y(:,0) = A(:,0)
  int COL = 1;
  char TRANS = 'T';
  M = NBAS; 
  N = NBAS;
  int NRHS = 1;
  LDA = NABS;
  LDB = NABS;
  int INFO = 0;

  simd_zero<double>(NABS,&V2[0]);
  simd_copy<double>(NBAS,&A(0,COL),&V2[0]);
  printf("values of H to recreate %lf %lf \n",A(0,COL),A(1,COL));
  simd_copy<double>(X.size(),&X[0],&XTMP[0]);
//  dgels_(&TRANS,&M,&N,&NRHS,&XTMP(0,COL),&LDA,&V2[0],&LDB,
//         &WORK[0],&LWORK,&INFO);

  gemat<double> XX(NBAS,NBAS);
  dgels_(&TRANS,&M,&N,&NRHS,&XX(0,COL),&LDA,&V2[0],&LDB,
         &WORK[0],&LWORK,&INFO);

  printf("\n output from dgels \n");
  V2.print();
  */

  int COL = 0;
  gemat<double>  XX(NBAS,NBAS);
  simd_copy<double>(NBAS,&A(0,COL),&V2[0]);
  XX(0,0) = X(0,0);
  XX(1,0) = X(0,1);
  XX(0,1) = X(1,0);
  XX(1,1) = X(1,1);
  M = NBAS;
  N = NBAS;
  int NRHS = 1;
  LDA = NBAS;
  LDB = NBAS;
  int INFO = 0;

  printf("\n XX matrix \n");
  XX.print();
  printf("\n col of A matrix? \n");
  V2.print(0,NBAS-1);

  dgesv_(&N,&NRHS,&XX(0,0),&LDA,&IPIV[0],&V2[0],&LDB,&INFO);

  printf("\n output from dgesv \n");
  V2.print();


  printf("\n the original vector? \n");
  for (int i=0;i<NBAS;i++)
  {
    V3(i) = V2(i)/X(i,COL);
  }
  V3.print(0,NBAS-1);
  return 0;

}
