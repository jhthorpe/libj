/*-----------------------------------------------------------
  linal_AUxpy.cpp
	JHT, January 7, 2022 : created

  .cpp file for AUxpy, which matrix multiplies a
  MxN matrix A (which may be scaled), 
  and NxN upper symmetric matrix U, a N vector x, and adds
  them to a (scaled) M vector y  **IN PLACE** 

  It is assumed that A,U,v, and y are held continuously 
  in memory, that A is stored column major, and that
  U is 
  
           A            U         x                y
         _______    ________     ___              ___
        | 0 3 5 |   \ 0 1 3 |   | 0 |            | 0 |
alpha*  | . . . |     \ . . |   | . |  += beta * | . |
        | . . . |       \ . |   | . |            | . |
         -------  .      ---  .  ---              ---

Parameters:
M	const long	rows of A,y
N	const long	cols of A,U,x
ALPHA	cosnt T		value to scale A^T with
A*	const T*	pointer to A
U*	const T*   	pointer to U
x*	const T*	pointer to x
BETA	const T		value to scale y with
y*	T*		pointer to y


-----------------------------------------------------------*/
#include "linal_AUxpy.hpp"

template <typename T>
void linal_AUxpy(const long M, const long N, const T ALPHA, 
                  const T* A, const T* U, const T* x,
                  const T BETA, T* y)
{

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set y to zero
    simd_zero<T>(M,y);

    //generate element of intermediate vector UX  
    for (long j=0;j<N;j++)
    {
      T TMP = 0;

      //elements j*(j+1)/2 -> j*(j+1)/2 + j in row are in order
      //note this is "reverse" order from normal USYM access
      TMP += simd_dot<T>(j+1,(U+(j*(j+1))/2),x); 

      //the rest of the elements are out of order
      for (long k=j+1; k<N ; k++)
      {
        TMP += *(U+(k*(k+1))/2 + j) * *(x+k);
      } 

      //multiply on A col is in order
      simd_axpy<T>(M,TMP,A+j*M,y);

    }//loop over intermediates U.x 

  //common case, alpha == 1 beta == 1
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL )

    //generate element of intermediate vector UX  
    for (long j=0;j<N;j++)
    {
      T TMP = 0;

      //elements j*(j+1)/2 -> j*(j+1)/2 + j in row are in order
      //note this is "reverse" order from normal USYM access
      TMP += simd_dot<T>(j+1,(U+(j*(j+1))/2),x); 

      //the rest of the elements are out of order
      for (long k=j+1; k<N ; k++)
      {
        TMP += *(U+(k*(k+1))/2 + j) * *(x+k);
      } 

      //multiply on A col is in order
      simd_axpy<T>(M,TMP,A+j*M,y);
    
  //General case
  } else {

    simd_scal_mul<T>(M,BETA,y);

    //generate element of intermediate vector UX  
    for (long j=0;j<N;j++)
    {
      T TMP = 0;

      //elements j*(j+1)/2 -> j*(j+1)/2 + j in row are in order
      //note this is "reverse" order from normal USYM access
      TMP += simd_dot<T>(j+1,(U+(j*(j+1))/2),x); 

      //the rest of the elements are out of order
      for (long k=j+1; k<N ; k++)
      {
        TMP += *(U+(k*(k+1))/2 + j) * *(x+k);
      } 

      //multiply on A col is in order
      simd_axpy<T>(M,ALPHA*TMP,A+j*M,y);

    }
  }

}

template void linal_AUxpy<double>(const long M, const long N, const double ALPHA, const double* A, const double* U, const double* x,const double BETA, double* y);
template void linal_AUxpy<float>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);
template void linal_AUxpy<long>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long* x,const long BETA, long* y);
template void linal_AUxpy<int>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);


/*
template <typename T, const int ALIGN>
void linal_AUxpy(const long M, const long N, const T ALPHA, 
                  const T* A, const T* U, const T* x,
                  const T BETA, T* y)
{

  //common case: alpha == 1, beta == 0
  if (fabs((double) ALPHA - (double) 1) < DZTOL &&
      fabs((double) BETA)               < DZTOL )
  {
    //set y to zero
    simd_zero<T,ALIGN>(M,y);

    //generate element of intermediate vector UX  
    for (long j=0;j<N;j++)
    {
      T TMP = 0;

      //elements j*(j+1)/2 -> j*(j+1)/2 + j in row are in order
      //note this is "reverse" order from normal USYM access
      TMP += simd_dot<T,ALIGN>(j+1,(U+(j*(j+1))/2),x); 

      //the rest of the elements are out of order
      for (long k=j+1; k<N ; k++)
      {
        TMP += *(U+(k*(k+1))/2 + j) * *(x+k);
      } 

      //multiply on A col is in order
      simd_axpy<T,ALIGN>(M,TMP,A+j*M,y);

    }//loop over intermediates U.x 

  //common case, alpha == 1 beta == 1
  } else if (fabs((double) ALPHA - (double) 1) < DZTOL &&
             fabs((double) BETA  - (double) 1) < DZTOL )

    //generate element of intermediate vector UX  
    for (long j=0;j<N;j++)
    {
      T TMP = 0;

      //elements j*(j+1)/2 -> j*(j+1)/2 + j in row are in order
      //note this is "reverse" order from normal USYM access
      TMP += simd_dot<T,ALIGN>(j+1,(U+(j*(j+1))/2),x); 

      //the rest of the elements are out of order
      for (long k=j+1; k<N ; k++)
      {
        TMP += *(U+(k*(k+1))/2 + j) * *(x+k);
      } 

      //multiply on A col is in order
      simd_axpy<T,ALIGN>(M,TMP,A+j*M,y);
    
  //General case
  } else {

    simd_scal_mul<T,ALIGN>(M,BETA,y);

    //generate element of intermediate vector UX  
    for (long j=0;j<N;j++)
    {
      T TMP = 0;

      //elements j*(j+1)/2 -> j*(j+1)/2 + j in row are in order
      //note this is "reverse" order from normal USYM access
      TMP += simd_dot<T,ALIGN>(j+1,(U+(j*(j+1))/2),x); 

      //the rest of the elements are out of order
      for (long k=j+1; k<N ; k++)
      {
        TMP += *(U+(k*(k+1))/2 + j) * *(x+k);
      } 

      //multiply on A col is in order
      simd_axpy<T,ALIGN>(M,ALPHA*TMP,A+j*M,y);

    }
  }

}

template void linal_AUxpy<double,128>(const long M, const long N, const double ALPHA, const double* A, const double* U, const double* x,const double BETA, double* y);
template void linal_AUxpy<double,64>(const long M, const long N, const double ALPHA, const double* A, const double* U, const double* x,const double BETA, double* y);
template void linal_AUxpy<double,32>(const long M, const long N, const double ALPHA, const double* A, const double* U, const double* x,const double BETA, double* y);
template void linal_AUxpy<double,16>(const long M, const long N, const double ALPHA, const double* A, const double* U, const double* x,const double BETA, double* y);
template void linal_AUxpy<double,8>(const long M, const long N, const double ALPHA, const double* A, const double* U, const double* x,const double BETA, double* y);

template void linal_AUxpy<float,128>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);
template void linal_AUxpy<float,64>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);
template void linal_AUxpy<float,32>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);
template void linal_AUxpy<float,16>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);
template void linal_AUxpy<float,8>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);
template void linal_AUxpy<float,4>(const long M, const long N, const float ALPHA, const float* A, const float* U, const float* x,const float BETA, float* y);

template void linal_AUxpy<long,128>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long* x,const long BETA, long* y);
template void linal_AUxpy<long,64>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long* x,const long BETA, long* y);
template void linal_AUxpy<long,32>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long* x,const long BETA, long* y);
template void linal_AUxpy<long,16>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long* x,const long BETA, long* y);
template void linal_AUxpy<long,8>(const long M, const long N, const long ALPHA, const long* A, const long* U, const long* x,const long BETA, long* y);

template void linal_AUxpy<int,128>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);
template void linal_AUxpy<int,64>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);
template void linal_AUxpy<int,32>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);
template void linal_AUxpy<int,16>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);
template void linal_AUxpy<int,8>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);
template void linal_AUxpy<int,4>(const long M, const long N, const int ALPHA, const int* A, const int* U, const int* x,const int BETA, int* y);
*/
