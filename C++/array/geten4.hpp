/*-------------------------------------------------------
  geten4.hpp
	JHT, December 15, 2021 : created 
  
  (GE)neral (TEN)sor dimension (4) : a general tensor
  with four dimensions, which can be assigned to or allocated with memory, 
  with optional alignment to BYTE boundaries 

  NOTE : this tensor is accessed in COLUMN-MAJOR order, 
         which this author believes to be superior.
	 That is, the loops run from the rightmost to the 
         leftmost index (as in Fortran). 

  NOTE : There is NO BOUNDS CHECKING in this class

  NOTE : Indexing begins at zero

  ACCESSING OPTIONS
  --------------------------
  M(i,j,k,l)		//access in COL-MAJOR order 
  M[x]			//access x'th element in buffer

  INITIALIZATION OPTIONS
  --------------------------
  geten4<double> M;     		//generate class, nothing else
  geten4<double> M(2,3,8,1);		//generate and allocate via malloc
  geten4<double> M(2,3,8,1,pntr);	//generates and assign location
  M.allocate(2,3,8,1);	        	//allocates via malloc 
  M.assign(2,3,8,1,pntr);	        //assigns buffer to address

  DEALLOCATION OPTIONS
  --------------------------
  M.deallocate();	//if allocated, free memory
  M.unassign();		//points buffer to NULL
  
  FUNCTIONS
  --------------------------
  M.print();		//prints whole tensor
  M.info();		//prints info about tensor
  M.size();		//returns number of elem (long)
  M.size_d1();		//returns number of dimension 1 (long)
  M.size_d2();		//returns number of dimension 2 (long)
  M.size_d3();		//returns number of dimension 3 (long)
  M.size_d4();		//returns number of dimension 4 (long)
  M.zero();		//zeros the whole tensor
  M.is_allocated();	//returns true if tensor is allocated
  M.is_assigned();	//returns true if tensor is assigned
  M = a;		//make the matrix equal to a scalar

--------------------------------------------------------*/
#ifndef GETEN4_HPP
#define GETEN4_HPP

#include <cstdlib> //for malloc
#include <cstdint> //for long
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <assert.h>//for assert

template <typename T>
class geten4
{
  private:
  T*               m_buf;	//pointer to start of memory
  T*               m_ptr;	//pointer to start of memory
  long             m_len;	//number of elements
  long             m_nd1;	//number of elements in d1
  long             m_nd2;	//number of elements in d2
  long             m_nd3;	//number of elements in d3
  long             m_nd4;	//number of elements in d4
  int        m_alignment; 	//alignment in BYTES
  bool       m_allocated;	//is allocated
  bool        m_assigned;	//is assigned

  public:
  //initialization/destructors
  geten4();							//empty constructor
  geten4(const long n, const long m, const long l, const long k);		//allocate 
  geten4(const long n, const long m, const long l, const long k, T* ptr);	//assign 
 ~geten4();							//destructor 

  //Operator overloading : inlined
  //reference to element i,j,k
  inline T& operator() (const long i, const long j, const long k, const long l)
    {return(*(m_buf + m_nd3*m_nd2*m_nd1*l + m_nd2*m_nd1*k + m_nd1*j + i));}
  //const reference to element i,j,k
  inline const T& operator() (const long i, const long j, const long k, const long l) const 
    {return(*(m_buf + m_nd3*m_nd2*m_nd1*l + m_nd2*m_nd1*k + m_nd1*j + i));}
  //reference i'th element
  inline T& operator[] (const long i)			
    {return(*(m_buf + i));}
  //const reference i'th element
  inline const T& operator[] (const long i) const
    {return(*(m_buf+i));}

  //Dimension information : inlined
  inline long size() const	//return length
    {return(m_len);}
  inline long size_d1() const	
    {return(m_nd1);}
  inline long size_d2() const
    {return(m_nd2);}
  inline long size_d3() const
    {return(m_nd3);}
  inline long size_d4() const
    {return(m_nd4);}


  //allocated/assigned : inlined
  inline bool is_allocated()
    {return m_allocated;}
  inline bool is_assigned()
    {return m_assigned;}
  inline int get_alignment()
    {return m_alignment;}

  //Class functions
  void aligned_allocate(const int ALIGN, const long n, const long m, const long l, const long k);
  void allocate(const long n, const long m, const long l, const long k);		//allocate memory 
  void free();						//free the memory
  void deallocate();					//deallocate memory
  void unassign();					//unassign to memory
  void assign(const long n, const long m, 
              const long l, const long k, T* ptr);	//assign to memory
  void reassign(const long n, const long m, const long l, 
              const long k, T* ptr);	//reassign to memory

  void info() const;		//print info
  void zero();	            	//makes tensor zero
  void operator= (const T a);	//makes tensor constant
  void print() const;		//prints tensor
  void calc_alignment();	//calculates the alignment of the tensor

};
template class geten4<double>;
template class geten4<float>;
template class geten4<long>;
template class geten4<int>;

#endif
