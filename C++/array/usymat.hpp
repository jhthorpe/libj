/*-------------------------------------------------------
  usymat.hpp
    JHT, October 28, 2021 : created 

  (U)pper (SY)mmetric (MAT)rix : 

  NOTE : this matrix is accessed in COLUMN-MAJOR order, 
         which this author believes to be superior. It
         only stores the elements in the upper-triangular 
         portion. 

  NOTE : Accessing the lower triangular elements is 
         undefined behavior

  NOTE : There is NO BOUNDS CHECKING in this class

  NOTE : Indexing begins at zero

  ACCESSING OPTIONS
  --------------------------
  M(i,j)		//access in COL-MAJOR order 
  M[i]			//access i'th element in m_buffer

  INITIALIZATION OPTIONS
  --------------------------
  usymat<double> M;     	    //generate class, nothing else
  usymat<double> M(3,3);	    //generate and allocate via malloc
  usymat<dobule> M(3,3,pntr);	//generates and assign location
  M.allocate(3,3);	            //allocates via malloc 
  M.assign(3,3,pntr);	        //assigns m_buffer to address
  M.aligned_allocate(32,N,N);   //alocates NxN matrix aligned to 32 bytes

  DEALLOCATION OPTIONS
  --------------------------
  M.deallocate();	//if m_allocated, free memory
  M.unassign();		//points m_buffer to NULL
  
  FUNCTIONS
  --------------------------
  M.print();		//prints whole matrix
  M.info();		//prints info about matrix
  M.size();		//returns number of elem (uint64_t)
  M.rows();		//returns number of rows (uint64_t)
  M.cols();		//returns number of cols (uint64_t)
  M.zero();		//zeros the whole matrix
  M.I();		//makes matrix the identity
  M.is_allocated();	//returns true if matrix is m_allocated
  M.is_assigned();	//returns true if matrix is m_assigned

--------------------------------------------------------*/
#ifndef USYMAT_HPP
#define USYMAT_HPP

#include <cstdlib> //for malloc
#include <stdio.h> //for printf
#include <limits>  //for numeric_limits::max()
#include <assert.h>//for assert

template <typename T>
class usymat
{
  private:
  T*           m_buf;			    //pointer to start of buffer 
  T*           m_ptr;               //pointer to start of memory
  long         m_len;				//number of elements
  long        m_ncol;				//number of rows
  int    m_alignment;               //alignment in bytes
  bool   m_allocated;                //is m_allocated
  bool    m_assigned;                //is m_assigned

  public:
  //initialization/destructors
  usymat();	//empty constructor
  usymat(const long n, const long m);		//allocate with row,col
  usymat(const long n, const long m, T* ptr);	//assign with row,col
 ~usymat();	//destructor 

  //Operator overloading : inlined
  inline T& operator() (const long i, const long j)	//ref elm i,j
    {return(*(m_buf+ j*(j+1)/2 + i));}
  inline const T& operator() (const long i, const long j) const 	//const elm i,j
    {return(*(m_buf+ j*(j+1)/2 + i));}
  inline T& operator[] (const long i)			//ref i'th element
    {return(*(m_buf + i));}
  inline const T& operator[] (const long i) const		//const i'th element
    {return(*(m_buf+i));}

  //Dimension information : inlined
  inline long size() const					//return m_length
    {return(m_len);}
  inline long rows() const					//return nrow
    {return(m_ncol);}
  inline long cols() const					//return m_ncol 
    {return(m_ncol);}

  //m_allocated/m_assigned : inlined
  inline bool is_allocated()
    {return m_allocated;}
  inline bool is_assigned()
    {return m_assigned;}
  inline int get_alignment()
    {return m_alignment;}

  //Class functions
  void aligned_allocate(const int ALIGN, const long n, const long m);
  void allocate(const long n, const long m);		//allocate memory 
  void assign(const long n, const long m, T* ptr);	//assign to memory
  void reassign(const long n, const long m, T* ptr);	//assign to memory
  void deallocate(); 		//deallocate memory
  void free();
  void unassign();		//unassign to memory

  void info() const;		//print info
  void zero();			//makes matrix zero
  void I();			//makes matrix identity
  void operator= (const T a);	//makes matrix constant
  void print() const;		//prints matrix
  void calc_alignment();	//determine alignment 

};

template class usymat<double>;
template class usymat<float>;
template class usymat<long>;
template class usymat<int>;

#endif
