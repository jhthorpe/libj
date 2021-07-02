/*-------------------------------------------------------
  dgemat.hpp
  Header file for dgemat class and functions, designed to
  deal with a general, double prescision matrix

  NOTE : this matrix is accessed in COLUMN-MAJOR order, 
         which this author believes to be superior

  This has two modes of access:
  (i,j) -- determines location from nrow,ncol  
  [i]   -- determines location from start + distance
--------------------------------------------------------*/
#ifndef DGEMAT_HPP
#define DGEMAT_HPP

#include <cstdlib> //for size_t
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()

class dgemat
{
  private:
  double*           buf;		//pointer to start of memory
  std::size_t       len;		//number of elements
  std::size_t      nrow;		//number of rows
  std::size_t      ncol;		//number of cols
  bool        allocated;                //is allocated
  bool         assigned;                //is assigned

  public:
  //initialization/destructors
  dgemat();									//empty constructor
  dgemat(const std::size_t n, const std::size_t m);				//allocate with row,col
  dgemat(const std::size_t n, const std::size_t m, double* ptr);		//assign with row,col
 ~dgemat();									//destructor 

  //Operator overloading
  double& operator() (const std::size_t i, const std::size_t j);	//ref elm i,j
  double operator() (const std::size_t i, const std::size_t j) const; 	//const elm i,j
  double& operator[] (const std::size_t i);				//ref i'th element
  double operator[] (const std::size_t i) const;			//const i'th element

  //Class functions
  void allocate(const std::size_t n, const std::size_t m);		//allocate memory 
  void deallocate();							//deallocate memory
  void assign(const std::size_t n,const std::size_t m, double* ptr);	//assign to memory
  void unassign();							//unassign to memory
  std::size_t size() const;						//return length
  std::size_t rows() const;						//return nrow
  std::size_t cols() const;						//return ncol 

  void info() const;							//print info
  void print() const;							//prints matrix
  
};

#endif
