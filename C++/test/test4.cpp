
#include "libj.hpp"

int main()
{
  const int NABS=4;
  const int NBAS=3;
  usymat<double> B(NBAS,NBAS);
  gemat<double>  X(NABS,NBAS);
  gemat<double>  Y(NABS,NBAS);
  gemat<double>  XTMP(NABS,NBAS);
  vec<double>    V(NABS);
  vec<double>    V2(NABS);

  V(0) = 0;
  V(1) = 1;
  V(2) = 2; 
  V(3) = 3; 
  
  X(0,0) = 1;
  X(1,0) = -11;
  X(2,0) = 1;
  X(3,0) = 1;
  X(0,1) = 2;
  X(1,1) = 4;
  X(2,1) = 6;
  X(3,1) = 8;
  X(0,2) = 4;
  X(1,2) = 16;
  X(2,2) = 36;
  X(3,2) = 64;

  //Form the test matrix A = X^T.V.X
  linal_DApB<double>(NABS,NBAS,1,&V[0],&X[0],0,&Y[0]);

  printf("V vector \n");
  V.print();
  printf("\n X matrix \n");
  X.print();
  printf("\n Y matrix \n");
  Y.print();
  
  //form upper triangular B
  linal_ATBpU<double>(NBAS,NBAS,NABS,1,&X[0],&Y[0],0,&B[0]);

  printf("\n B matrix \n");
  B.print(); 

  linal_ATDApU<double>(NBAS,NABS,&X[0],1,&V[0],0,&B[0]);
  printf("\n faster B matrix \n");
  B.print();

  //get lwork
  long LWORK = linal_ATDAeU_LWORK(NBAS,NABS);
  double RMS;
  vec<double> WORK(LWORK);
  linal_ATDAeU(NBAS,NABS,&X[0],&V[0],&B[0],RMS,&WORK[0]);

  //Disturbing the matix
  printf("\n modifying diagonal elements of B \n");
  double w = 10;
  usymat<double> BB(NBAS,NBAS);
  vec<double>VV(NBAS);
  simd_copy(B.size(),&B[0],&BB[0]);
  for (int i=0;i<NBAS;i++)
  {
    BB(i,i) += w*(i+0.5);
  }
  BB.print();

  linal_ATDAeU(NBAS,NABS,&X[0],&VV[0],&BB[0],RMS,&WORK[0]);
  usymat<double>BP(NBAS,NBAS);
  linal_ATDApU<double>(NBAS,NABS,&X[0],1,&VV[0],0,&BP[0]);
  BP.print();

 //TESTING BREAKUP
 printf("\n ");
 printf("\n testing breakup... \n");
 usymat<double> BD(NBAS,NBAS);
 vec<double> VBD(NABS);
 simd_zero(BD.size(),&BD[0]);
 for (int i=0;i<NBAS;i++)
 {
   BD(i,i) = w*(i+0.5);  
 }
 printf("\n Diagonal elements \n");
 BD.print();
 LWORK = linal_ATDAeU_LWORK(NBAS,NABS);
 vec<double> WORK2(LWORK);
 linal_ATDAeU(NBAS,NABS,&X[0],&VBD[0],&BD[0],RMS,&WORK2[0]);

 //Now, does the addition of BD and V result in the right matrix?
 usymat<double>BBB(NBAS,NBAS);
 vec<double>VVV(NABS);
 simd_elemwise_add<double>(NABS,&V[0],&VBD[0],&VVV[0]);
 linal_ATDApU<double>(NBAS,NABS,&X[0],1,&VVV[0],0,&BBB[0]);
 printf("\n this is the new matrix \n"); 
 BBB.print();

 printf("\n using original? \n");
 
  
  
  return 0;
}
