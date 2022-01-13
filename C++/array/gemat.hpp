/*-------------------------------------------------------
  gemat.hpp
	JHT, October 28, 2021 : created 
  
  (GE)neral (MAT)rix : COL-MAJOR, general matrix, 
  which can be assigned to or allocated with memory, 
  with optional alignment to BYTE boundaries 

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
  gemat<double> M;     	        //generate class, nothing else
  gemat<double> M(2,3);	        //generate and allocate via malloc
  gemat<double> M(2,3,pntr);	//generates and assign location
  M.allocate(2,3);	            //allocates via malloc 
  M.assign(2,3,pntr);	        //assigns buffer to address

  DEALLOCATION OPTIONS
  --------------------------
  M.deallocate();	//if allocated, free memory
  M.unassign();		//points buffer to NULL
  
  FUNCTIONS
  --------------------------
  M.print();		//prints whole matrix
  M.info();		//prints info about matrix
  M.size();		//returns number of elem (long)
  M.rows();		//returns number of rows (long)
  M.cols();		//returns number of cols (long)
  M.zero();		//zeros the whole matrix
  M.I();		//makes matrix the identity
  M.is_allocated();	//returns true if matrix is allocated
  M.is_assigned();	//returns true if matrix is assigned

--------------------------------------------------------*/
#ifndef GEMAT_HPP
#define GEMAT_HPP

#include <cstdlib> //for malloc
#include <cstdint> //for long
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <assert.h>//for assert

template <typename T>
class gemat
{
  private:
  T*               m_buf;	//pointer to start of buffer
  T*               m_ptr;	//pointer to location that can be deallocated
  long             m_len;	//number of elements
  long            m_nrow;	//number of rows
  long            m_ncol;	//number of cols
  int        m_alignment;	//alignment in BYTES
  bool       m_allocated;	//is allocated
  bool        m_assigned;	//is assigned

  public:
  //initialization/destructors
  gemat();					//empty constructor
  gemat(const long n, const long m);		//allocate with row,col
  gemat(const long n, const long m, T* ptr);	//assign with row,col
 ~gemat();					//destructor 

  //Operator overloading : inlined
  inline T& operator() (const long i, const long j)	//ref elm i,j
    {return(*(m_buf+m_nrow*j+i));}
  inline const T& operator() (const long i, const long j) const	//const elm i,j
    {return(*(m_buf+m_nrow*j+i));}
  inline T& operator[] (const long i)	//ref i'th element
    {return(*(m_buf + i));}
  inline const T& operator[] (const long i) const	//const i'th element
    {return(*(m_buf+i));}

  //Dimension information : inlined
  inline long size() const	//return length
    {return(m_len);}
  inline long rows() const	//return nrow
    {return(m_nrow);}
  inline long cols() const	//return ncol 
    {return(m_ncol);}

  //allocated/assigned : inlined
  inline bool is_allocated()
    {return m_allocated;}
  inline bool is_assigned()
    {return m_assigned;}
  inline int get_alignment()
    {return m_alignment;}

  //Class functions
  void aligned_allocate(const int ALIGN, const long n, const long m);
  void allocate(const long n, const long m);	    //allocate memory 
  void free();                                      //free the memory
  void deallocate();	//deallocate memory
  void unassign();	//unassign to memory
  void assign(const long n, const long m, T* ptr);	//assign to memory
  void reassign(const long n, const long m, T* ptr);	//reassign to memory

  void info() const;	//print info
  void zero();	//makes matrix zero
  void I();	//makes matrix identity
  void operator= (const T a);	//makes matrix constant
  void print() const;	//prints matrix
  void calc_alignment();

};
template class gemat<double>;
template class gemat<float>;
template class gemat<long>;
template class gemat<int>;
template class gemat<double*>;

#endif
