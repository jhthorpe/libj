/*-------------------------------------------------------
  dvec.hpp
  
  (D)ouble precision, (VEC)tor

  NOTE : There is NO BOUNDS CHECKING in this class

  NOTE : Indexing begins at zero 

  ACCESSING OPTIONS
  --------------------------
  v(i)                  //access i'th element in buffer
  v[i]			//access i'th element in buffer

  INITIALIZATION OPTIONS
  --------------------------
  dvec v;		//generate class, nothing else
  dvec v(5);		//generate and allocate via malloc
  dvec v(5,pntr);	//generates and assign location
  v.allocate(5);	//allocates via malloc 
  v.assign(5,pntr);	//assigns buffer to address

  DEALLOCATION OPTIONS
  --------------------------
  v.deallocate();	//if allocated, free memory
  v.unassign();		//points buffer to NULL
  
  FUNCTIONS
  --------------------------
  v.print();		//prints whole matrix
  v.info();		//prints info about matrix
  v.zero();		//zeros the whole matrix
  v.size();		//returns number of elem (uint64_t)

--------------------------------------------------------*/
#ifndef DVEC_HPP
#define DVEC_HPP

#include <cstdlib>
#include <cstdint> //for integer types
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <assert.h>  //for assert

class dvec
{
  private:
  double*           buf;		//pointer to start of memory
  long              len;		//number of elements
  bool        allocated;                //is allocated
  bool         assigned;                //is assigned

  public:
  //initialization/destructors
  dvec();					//empty constructor
  dvec(const long n);				//construct with n elements
  dvec(const long n, double* ptr);		//construct with n elements
 ~dvec();					//destructor 

  //Operator overloading
  double& operator() (const long i);	 	//ref elm i
  double operator() (const long i) const; 	//const elm i

  //Class functions
  void allocate(const long n);			//allocate memory 
  void deallocate();				//deallocate memory
  void assign(const long n, double* ptr);	//assign to memory
  void unassign();				//unassign to memory
  long size() const;				//return length
  void info() const;				//print info
  void print() const;				//prints the elements of the vector
  void zero();					//zeros the vector
};

#endif
