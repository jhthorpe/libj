/*-------------------------------------------------------
  vec.hpp
  
  (VEC)tor : a lightweight vector implementation that 
  allows for assignement to existing memory, as well as
  allocation (aligned or otherwise) via malloc

  NOTE : There is NO BOUNDS CHECKING in this class

  NOTE : Indexing begins at zero, and all elements
         are stored continuously in memory. 

  ACCESSING OPTIONS
  --------------------------
  v(i)          //access i'th element in buffer
  v[i]			//access i'th element in buffer

  INITIALIZATION OPTIONS
  --------------------------
  vec<double> v;                //generate double vector, nothing else
  vec<double> v(5);	            //generate and allocate a double via malloc
  vec<double> v(5,ptr);         //generate and assign a double vector to location
  v.allocate(5);	            //allocates vector via malloc 
  v.assign(5,pntr);	            //assigns buffer to address
  v.alligned_allocate(BYTE,5)   //alligned allocation via malloc to BYTE 

  DEALLOCATION OPTIONS
  --------------------------
  v.deallocate();	//if allocated, free memory
  v.unassign();		//points buffer to NULL
  v.free();         //frees memory
  
  FUNCTIONS
  --------------------------
  v.info();		    //prints info about matrix
  v.zero();		    //zeros the whole matrix
  v.size();		    //returns number of elem (long)
  v.is_allocated();	//returns true if vector is allocated
  v.is_assigned();	//returns true if vector is assigned
  v.alignment();    //returns alignment in BYTES

--------------------------------------------------------*/
#ifndef VEC_HPP
#define VEC_HPP

#include <cstdlib>
#include <cstdint> //for integer types
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <assert.h>  //for assert

template <typename T>
class vec
{
  private:
  T*         m_buf;         //buffer start
  T*         m_ptr;         //malloc pointer
  long       m_len;         //number of elements
  bool       m_allocated;   //is allocated
  bool       m_assigned;    //is assigned
  int        m_alignment;  //alignment in BYTES

  public:
  //initialization/destructors
  vec();					        //empty constructor
 ~vec();					        //destructor 

  vec(const long n);				//construct with n elements
  vec(const long n, T* ptr);	    //construct with n elements at ptr

  //Operator overloading : inlined
  inline T& operator() (const long i)	 //ref elm i
    {return(*(m_buf+i));} 
  inline const T& operator() (const long i) const //const elm i
    {return(*(m_buf+i));} 
  inline T& operator[] (const long i)	 //ref elm i
    {return(*(m_buf+i));} 
  inline const T& operator[] (const long i) const	//const elm i
    {return(*(m_buf+i));} 				

  //Size function : inlined
  inline long size() const 			//return length
  {return(m_len);}

  //allocated/assigned : inlined
  inline bool is_allocated() const
    {return m_allocated;}
  inline bool is_assigned() const
    {return m_assigned;}
  inline int get_alignment() const
    {return m_alignment;}
  /*

  //Class functions
  */
  void allocate(const long n);			//allocate memory 
  void aligned_allocate(const int ALIGN, 
                        const long N); 		//allocate aligned memory
  void assign(const long n, T* ptr);    	//assign to memory
  void reassign(const long n, T* ptr);		//assign to new mem
  void deallocate();				//deallocate memory
  void unassign();				//unassign to memory
  void free();					//frees memory
  void info() const;				//print info
  void zero();					//zeros the vector
  void operator= (const T a);			//makes vector equal constant
  void print() const;				//prints the elements of the vector
  void print(const long lo,		//prints elements of vector between 
             const long hi) const;         	//  lo and hi
  void calc_alignment(); 			//determine the alignment

};

template class vec<double>;
template class vec<float>;
template class vec<long>;
template class vec<int>;

#endif
