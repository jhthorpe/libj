/*-------------------------------------------------------
  dcore.hpp 
  Header file for dcore class and related functions.	
--------------------------------------------------------*/
#ifndef DCORE_HPP
#define DCORE_HPP

#include <cstdlib> //for size_t
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <algorithm> // for min()

class dcore
{
private:
  double*     buf; 	//pointer to start of memory
  double*     next;	//next free element
  std::size_t len;	//number of elements in array
  std::size_t navbl;	//number of free elements
  bool    allocated;	//bool for if class is allocated

public:
  //initialization/destructors
  dcore();		//constructor
  dcore(std::size_t n); //constructor and allocator
 ~dcore();		//destructor

  //Operator overloading
  double& operator() (const std::size_t i); 	 //reference elem i
  double operator() (const std::size_t i) const; //value of elem i
  
  //Class functions
  void allocate(const std::size_t n);
  void deallocate();
  std::size_t size() const;			//returns num elements
  std::size_t nfree() const;			//number of free elm
  void info() const;				//prints info
  double* checkout (const std::size_t n);	//checks out n elements
  double* remove   (const std::size_t n);	//checks out n elements
  
};

#endif
