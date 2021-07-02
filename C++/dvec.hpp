/*-------------------------------------------------------
  dvec.hpp
  Header file for dvec class and functions, designed to
  deal with double precision vectors
--------------------------------------------------------*/
#ifndef DVEC_HPP
#define DVEC_HPP

#include <cstdlib> //for size_t
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()

class dvec
{
  private:
  double*           buf;		//pointer to start of memory
  std::size_t       len;		//number of elements
  bool        allocated;                //is allocated
  bool         assigned;                //is assigned

  public:
  //initialization/destructors
  dvec();					//empty constructor
  dvec(const std::size_t n);			//construct with n elements
  dvec(const std::size_t n, double* ptr);	//construct with n elements
 ~dvec();					//destructor 

  //Operator overloading
  double& operator() (const std::size_t i);	 //ref elm i
  double operator() (const std::size_t i) const; //const elm i

  //Class functions
  void allocate(const std::size_t n);			//allocate memory 
  void deallocate();					//deallocate memory
  void assign(const std::size_t n, double* ptr);	//assign to memory
  void unassign();					//unassign to memory
  std::size_t size() const;				//return length
  void info() const;					//print info
  void print() const;					//prints the elements of the vector
  
};

#endif
