/*-------------------------------------------------------
  Core.hpp 
	JHT, December 19, 2021 : created 

  (CORE) memory

  A lightweight, linear memory manager template 

  INITIALIZATION
  --------------------------
  Core buf;		//empty initialization
  Core buf(n);		//initialize and allocate n (long) elements

  ALLOCATION/DEALLOCATTION
  --------------------------
  buf.allocate(n);	//allocates n elements via malloc for Core
  buf.deallocate();    //frees buffer for Core
  buf.assign(n,ptr);   //assigns buffer to preallocated memory
  buf.unassign();      //unassigns buffer 

  INFORMATION
  --------------------------
  buf.size();		//returns long number of elements
  buf.nfree();		//returns long number of free elements
  buf.info();		//prints information about Core
  buf.is_allocated();	//returns true if allocated

  RESERVING DATA
  --------------------------
  buf.aligned_checkout(64,n) //checkouts of memory aligned by BYTES
  buf.checkout(n);	//returns a pointer to start of a reserved section 
  buf.remove(n);	//returns n-elements at the end of the buffer  

  TAKING AWAY DATA
  -------------------------
  buf.take_free(n);	//removes some free data from the Core
  buf.return_free(n);	//returns some free data to the Core
  
--------------------------------------------------------*/
#ifndef CORE_HPP
#define CORE_HPP

#include <cstdlib> //for malloc
#include <cstdint> //for long
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()

template <typename T>
class Core
{
private:
  T*	            buf; 			//pointer to start of memory
  T*	           next;			//addresss of next free element
  long              len;			//number of elements in array
  long            navbl;			//number of free elements
  long            ntake;			//number of taken elements
  bool        allocated;			//bool for if class is allocated
  bool         assigned;

public:
  //initialization/destructors
  Core();					//constructor
  Core(long n); 				//constructor and allocator
  Core(long n, T* ptr); 			//construct and assign 
 ~Core();					//destructor

  //Operator overloading : inlined
  inline T& operator() (const long i) 	 //reference elem i
    {return (*(buf+i));}
  inline const T& operator() (const long i) const //value of elem i
    {return (*(buf+i));}
  inline T& operator[] (const long i) 	 //reference elem i
    {return (*(buf+i));}
  inline const T& operator[] (const long i) const //value of elem i
    {return (*(buf+i));}
  
  //Size information : inlined
  inline long size() const			//returns num elements
    {return len;}
  inline long nfree() const			//number of free elm
    {return navbl;}
  inline bool is_allocated() const		//return if allocated
    {return allocated;}
  inline bool is_assigned() const		//return if allocated
    {return assigned;}
  
  //Class functions
  void allocate(const long n);
  void deallocate();
  void assign(const long n, T* ptr);
  void unassign();
  void info() const;				//prints info
  T* checkout(const long n);			//checks out n elements
  int remove(const long n);			//removes n elements
  void print() const;				//print the elements
  int take_free(const long n);
  int return_free(const long n);
  T* aligned_checkout(const long ALIGN, const long n); //checks out memory aligned by ALIGN bytes 
  
};

template class Core<double>;
template class Core<float>;
template class Core<long>;
template class Core<int>;
template class Core<double*>;

#endif
