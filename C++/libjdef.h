/*--------------------------------------------------------
  libjdef.h
	JHT, Febuary 4, 2022 : created

  .h file which defines the OpenMP, MPI, and other 
  things for compiling libj 
--------------------------------------------------------*/

//OpenMP is easy
#if defined (_OPNEMP)
  #define OMP 1
#endif

//We must manually set if we want MPI
// If you do not want MPI, comment-out the following line
#define MPI 1
