/*-------------------------------------------------------
  dgemat.hpp
	JHT, July 2, 2021
  
  (D)ouble precision, (GE)neral (MAT)rix

  NOTE : this matrix is accessed in COLUMN-MAJOR order, 
         which this author believes to be superior. 

  NOTE : There is NO BOUNDS CHECKING in this class

  NOTE : Indexing begins at zero

  ACCESSING OPTIONS
  --------------------------
  M(i,j)		//access in COL-MAJOR order 
  M[i]			//access i'th element in buffer

  INITIALIZATION OPTIONS
  --------------------------
  dgemat M;     	//generate class, nothing else
  dgemat M(2,3);	//generate and allocate via malloc
  dgemat M(2,3,pntr);	//generates and assign location
  M.allocate(2,3);	//allocates via malloc 
  M.assign(2,3,pntr);	//assigns buffer to address

  DEALLOCATION OPTIONS
  --------------------------
  M.deallocate();	//if allocated, free memory
  M.unassign();		//points buffer to NULL
  
  FUNCTIONS
  --------------------------
  M.print();		//prints whole matrix
  M.info();		//prints info about matrix
  M.size();		//returns number of elem (uint64_t)
  M.rows();		//returns number of rows (uint64_t)
  M.cols();		//returns number of cols (uint64_t)
  M.zero();		//zeros the whole matrix
  M.I();		//makes matrix the identity

--------------------------------------------------------*/
#ifndef DGEMAT_HPP
#define DGEMAT_HPP

#include <cstdlib> //for malloc
#include <cstdint> //for uint64_t
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <assert.h>//for assert

class dgemat
{
  private:
  double*           buf;					//pointer to start of memory
  long              len;					//number of elements
  long             nrow;					//number of rows
  long             ncol;					//number of cols
  bool        allocated;                			//is allocated
  bool         assigned;                			//is assigned

  public:
  //initialization/destructors
  dgemat();							//empty constructor
  dgemat(const long n, const long m);				//allocate with row,col
  dgemat(const long n, const long m, double* ptr);		//assign with row,col
 ~dgemat();							//destructor 

  //Operator overloading
  double& operator() (const long i, const long j);		//ref elm i,j
  double operator() (const long i, const long j) const; 	//const elm i,j
  double& operator[] (const long i);				//ref i'th element
  double operator[] (const long i) const;			//const i'th element

  //Class functions
  void allocate(const long n, const long m);			//allocate memory 
  void deallocate();						//deallocate memory
  void assign(const long n, const long m, double* ptr);		//assign to memory
  void assign(double* ptr);					//assign to memory
  void unassign();						//unassign to memory
  long size() const;						//return length
  long rows() const;						//return nrow
  long cols() const;						//return ncol 

  void info() const;						//print info
  void print() const;						//prints matrix
  void zero();							//makes matrix zero
  void I();							//makes matrix identity
  
};

#endif
