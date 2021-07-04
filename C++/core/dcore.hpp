/*-------------------------------------------------------
  dcore.hpp 
	JHT, July 2, 2021

  (D)ouble precision (CORE) memory

  A lightweight double precision memory manager. 

  NOTE : all indices are explicitly unsigned, 64-bit (8-byte) values

  INITIALIZATION
  --------------------------
  dcore dbuf;		//empty initialization
  dcore dbuf(n);	//initialize and allocate n (uint64_t) elements

  ALLOCATION/DEALLOCATTION
  --------------------------
  dbuf.allocate(n);	//allocates n elements via malloc for dcore
  dbuf.deallocate();    //frees buffer for dcore

  INFORMATION
  --------------------------
  dbuf.size();		//returns uint64_t number of elements
  dbuf.nfree();		//returns uint64_t number of free elements
  dbuf.info();		//prints information about dcore

  RESERVING DATA
  --------------------------
  dbuf.checkout(n);	//returns a pointer to start of free, n-element section
  dbuf.remove(n);	//returns n-elements at the end of the buffer  
  
--------------------------------------------------------*/
#ifndef DCORE_HPP
#define DCORE_HPP

#include <cstdlib> //for malloc
#include <cstdint> //for uint64_t
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()

class dcore
{
private:
  double*           buf; 			//pointer to start of memory
  double*          next;			//next free element
  long              len;			//number of elements in array
  long            navbl;			//number of free elements
  bool        allocated;			//bool for if class is allocated

public:
  //initialization/destructors
  dcore();					//constructor
  dcore(long n); 				//constructor and allocator
 ~dcore();					//destructor

  //Operator overloading
  double& operator() (const long i); 	 	//reference elem i
  double operator() (const long i) const; 	//value of elem i
  
  //Class functions
  void allocate(const long n);
  void deallocate();
  long size() const;				//returns num elements
  long nfree() const;				//number of free elm
  void info() const;				//prints info
  double* checkout (const long n);		//checks out n elements
  double* remove   (const long n);		//checks out n elements
  
};

#endif
