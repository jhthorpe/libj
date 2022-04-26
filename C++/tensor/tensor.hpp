/*----------------------------------------------------------------------------
  tensor.hpp
	JHT, April 10, 2022 : created

  .hpp file for the general tensor class. This behaves similarly to 
  std::array in that it cannot be grown dynamically, though it can be 
  allocated (similar to a fortran dimension). 

  The storage is column major

  Note that the index access via () is implemented via a varadic template, 
  and thus should be rather efficient, though [], which accesses simply a 
  displacement from the buffer pointer, is the most efficient provided you are
  certain you are using it correctly! 


  INITIALIZATION
  -------------------
  Create, but do not assign or allocate
    libj::tensor<double> T; 

  Create and allocate with set legnths via malloc
    libj::tensor<double> A(1,4,3);

  Create and assign to memory with set lengths
    libj::tensor<double> A(pointer,1,4,3);

  Allocate or assign an existing tensor:
    T.allocate(1,4,3);
    T.aligned_allocate(64,1,4,3); //where 64 is the byte alignment 
    T.assign(1,4,3,pointer);

  Deallocate
    T.deallocate();
    T.unassign();

  Reassignment (including reshaping)
    T.assign(pointer, 2,5,1);
  

  ELEMENT ACCESS
  ------------------
  To access the n'th element
    T[N];

  To use the index access
    T(1,2,3);

  To use vector access
    T({1,2,3});

  USEFUL FUNCTIONS
  --------------------
    can_alias(T1,T2);		//returns true if two tensors can alias
    same_shape<double>(T1,T2);	//returns true if T1 and T2 (double tensors) 
            			//  are the same shape, false otherwise
    T.dim();			//returns number of dimensions
    T.size();			//returns total number of elements
    T.size(2);			//returns size of dimension 2 (3rd dimension)
    T.stride(1);		//returns stride of dimension 1 (2nd dimension)
    T.is_allocated();		//returns true if allocated
    T.is_assigned();		//returns true if assigned
    T.is_set();			//returns true if allocated or assigned
    T.is_sequential();		//returns true if elements of tensor are sequential
    T.block(i,j,k,		//returns a tensor that access a block of tensor T
            l0,l1,l1);		// starting at (i,j,k) and dimension (l0,l1,l2) 
    auto T1 = T2.block(...);	//makes a new tensor out of a block of an old
				// tensor 	
----------------------------------------------------------------------------*/
#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>

//This defines alignments
#include "libjdef.h"
#include "alignment.hpp"

namespace libj 
{

template<typename T>
class tensor
{

  private:
  T*                  M_BUFFER;        //start of data
  T*                  M_POINTER;       //pointer to malloc	
  std::vector<size_t> M_LENGTHS;       //vector of lengths 
  std::vector<size_t> M_STRIDE;        //pointer to strides
  size_t	      M_NDIM;          //number of dimensions
  size_t              M_NELM;          //total number of elements
  size_t              M_ALIGNMENT;     //alignment in bytes
  bool                M_IS_ALLOCATED;  //tensor is allocated with malloc 
  bool                M_IS_ASSIGNED;   //tensor is assigned with malloc 
  bool                M_IS_SEQUENTIAL; //tensor elements are sequential

  //internal functions
  void m_set_default();
  void m_allocate();
  void m_aligned_allocate(const size_t BYTES);
  void m_assign(T* pointer);
  void m_sequential();

  //internal varadic templates for data access
  template<class...Rest>
  size_t m_index(const size_t level, const size_t first, const Rest...rest) const
  {
    return first*M_STRIDE[level] + m_index(level+1,rest...); 
  }
  size_t m_index(const size_t level, const size_t first) const
  {
    return first*M_STRIDE[level];
  }

  //internal varadic templates for initialization
  void m_init()
  {
    M_NDIM = M_LENGTHS.size();
    for (size_t i=0;i<M_NDIM;i++)
    {
      if (M_LENGTHS[i] <= 0)
      {
        printf("ERROR libj::tensor::m_init\n");
        printf("Dimension %zu has length <= 0 \n",i);
        exit(1);
      }
    }
  }
  void m_init(const size_t first)
  {
    M_STRIDE.push_back(M_NELM);
    M_LENGTHS.push_back(first);
    M_NELM *= first;
    m_init();
  }
  template <class...Rest> void m_init(const size_t first, const Rest...rest)
  {
    M_STRIDE.push_back(M_NELM);
    M_LENGTHS.push_back(first);
    M_NELM *= first;
    m_init(rest...);
  }

  //Internal varadic templates for block
  template<class...Rest> 
  void m_block(const size_t level, std::vector<size_t>& start,
               std::vector<size_t>& length, const size_t first,
               const Rest...rest) const;
  void m_block(const size_t level, std::vector<size_t>& start,
               std::vector<size_t>& length, const size_t last) const;
  
  public:

  //Constructors and destructors
  tensor();			
  ~tensor();			

  //Allocate constructor
  template<class...Rest> tensor(const size_t first,const Rest...rest);	

  //Assign constructor
  template<class...Rest> tensor(T* pointer, const size_t first,const Rest...rest);

  //Equals constructor
  tensor(const tensor<T>& other);

  //allocate,assign
  template<class...Rest> void allocate(const size_t first,const Rest...rest);
  template<class...Rest> void aligned_allocate(const size_t BYTES, const size_t first,
                                               const Rest...rest);
  template<class...Rest> void assign(T* pointer, const size_t first,const Rest...rest);
  void deallocate();
  void unassign();

  //equals assign
  void operator= (const tensor<T>& other);

  //Getters
  const size_t  size() const {return M_NELM;}
  const size_t  size(const size_t dim) const {return M_LENGTHS[dim];}
  const size_t  dim() const {return M_NDIM;}
  const size_t  alignment() const {return M_ALIGNMENT;}
  const size_t  stride(const size_t dim) const {return M_STRIDE[dim];}
  const bool    is_allocated() const {return M_IS_ALLOCATED;}
  const bool    is_assigned() const {return M_IS_ASSIGNED;}
  const bool    is_set() const {return M_IS_ALLOCATED || M_IS_ASSIGNED;}
  const bool    is_sequential() const {return M_IS_SEQUENTIAL;}

  //Access functions
  T& operator[] (const size_t stride) {return *(M_BUFFER+stride);}
  const T& operator[] (const size_t stride) const {return *(M_BUFFER+stride);}

  template<class...Rest> T& operator() (const size_t i0,const Rest...rest)
  {
    return *(M_BUFFER + i0*M_STRIDE[0] + m_index(1,rest...));
  }

  template<class...Rest> const T& operator() (const size_t i0,const Rest...rest) const
  {
    return *(M_BUFFER + i0*M_STRIDE[0] + m_index(1,rest...));
  }

  T& operator() (const std::vector<size_t> vec)
  {
    size_t offset = 0;
    for (size_t dim=0;dim<M_NDIM;dim++) {offset += M_STRIDE[dim]*vec[dim];}
    return *(M_BUFFER+offset);
  }
  const T& operator() (const std::vector<size_t> vec) const
  {
    size_t offset = 0;
    for (size_t dim=0;dim<M_NDIM;dim++) {offset += M_STRIDE[dim]*vec[dim];}
    return *(M_BUFFER+offset);
  }

  //Create a block
  template<class...Rest> tensor<T> block(const size_t first, const Rest...rest) const;
//  tensor<T> block(const size_t first, ...) const;

  template<typename T1, typename T2> friend bool can_alias(const tensor<T1>& A, 
                                                           const tensor<T2>& B);
}; //end of normal tensor

//-----------------------------------------------------------------------
// returns true if the tensors are the same shape
//-----------------------------------------------------------------------
template <typename T>
bool same_shape(const tensor<T>& A, const tensor<T>& C)
{
  if (A.dim() != C.dim()) {return false;}
  for (int dim=0;dim<A.dim();dim++)
  {
    if (A.size(dim) != C.size(dim)) {return false;}
    if (A.stride(dim) != C.stride(dim)) {return false;}
  }
  return true;
}

//-----------------------------------------------------------------------
// returns true if two tensors can alias 
//-----------------------------------------------------------------------
template <typename T1, typename T2>
bool can_alias(const tensor<T1>& A, const tensor<T2>& B)
{
  const size_t a = (size_t) A.M_BUFFER;
  const size_t b = (size_t) (A.M_BUFFER + A.M_NELM);
  const size_t x = (size_t) B.M_BUFFER;
  const size_t y = (size_t) (B.M_BUFFER + B.M_NELM);
  
  if (  (x <= a && a <= y) || (x <= b && b <= y) 
     || (a <= x && x <= b) || (a <= y && y <= b))
  {
    return true;
  } else {
    return false;
  }
}

//-----------------------------------------------------------------------
// Initialization functions
//-----------------------------------------------------------------------
template<typename T>
void tensor<T>::m_set_default()
{ 
  M_BUFFER = NULL;
  M_POINTER = NULL;
  M_NELM = 0; 
  M_ALIGNMENT = 0; 
  M_IS_ALLOCATED = false;
  M_IS_ASSIGNED = false;
  M_IS_SEQUENTIAL = false;
  M_LENGTHS.clear();
  M_STRIDE.clear();
  
}

//-----------------------------------------------------------------------
//Blank contructor
//-----------------------------------------------------------------------
template<typename T>
tensor<T>::tensor()
{
  m_set_default();
}

//-----------------------------------------------------------------------
// blank destructor
//-----------------------------------------------------------------------
template<typename T>
tensor<T>::~tensor()
{
  if (M_IS_ALLOCATED) {deallocate();}
}

//-----------------------------------------------------------------------
//Allocator constructor
//-----------------------------------------------------------------------
template<typename T> template<class...Rest>
tensor<T>::tensor(const size_t first,const Rest...rest)
{
  //set the default
  m_set_default();

  //initialize
  M_LENGTHS.push_back(first);
  M_STRIDE.push_back(1);
  M_NELM = first; 
  m_init(rest...);//performs rest of varadic initialization

  //call the internal allocate function
  m_allocate();
  
}

//-----------------------------------------------------------------------
//Assignment constructor
//-----------------------------------------------------------------------
template<typename T> template<class...Rest>
tensor<T>::tensor(T* pointer, const size_t first,const Rest...rest)
{
  //set the default
  m_set_default();

  //Initialize
  M_LENGTHS.push_back(first);
  M_STRIDE.push_back(1);
  M_NELM = first; 
  m_init(rest...);//performs rest of varadic initialization

  //call the internal allocate function
  m_assign(pointer);
}

//-----------------------------------------------------------------------
// internal malloc
//-----------------------------------------------------------------------
template<typename T>
void tensor<T>::m_allocate()
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    M_POINTER = (T*) malloc(sizeof(T)*M_NELM);
    M_BUFFER = M_POINTER;
    if (M_BUFFER == NULL || M_POINTER == NULL)
    {
      printf("ERROR libj::tensor::m_allocate could not allocate M_BUFFER \n");
      exit(1);
     }
     M_IS_ALLOCATED = true;
     M_ALIGNMENT = libj::calc_alignment((void*) M_BUFFER);
    
     //determine if the memory is sequential
     m_sequential();

   } else {
     printf("ERROR libj::tensor::m_allocate\n");
     printf("attempted to allocate an already set vector\n");
     exit(1);
   }
}

//-----------------------------------------------------------------------
// internal assign
//-----------------------------------------------------------------------
template<typename T>
void tensor<T>::m_assign(T* pointer)
{
  if (!M_IS_ALLOCATED)
  {
    M_BUFFER = pointer;
    M_POINTER = pointer;
    M_IS_ASSIGNED = true;
    M_ALIGNMENT = libj::calc_alignment((void*) M_BUFFER);
    m_sequential();
  } else {
    printf("ERROR libj::tensor::m_assign\n");
    printf("attempted to assign an already allocated vector\n");
    exit(1);
   }
}

//----------------------------------------------------------------------------
// internal aligned_allocate 
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::m_aligned_allocate(const size_t ALIGN)
{
  //check we are not allocated or assigned
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    //check alignment is divisible by 2
    if (ALIGN%2 != 0)
    {
      printf("ERROR libj::tensor::m_aligned_allocate\n");
      printf("input BYTE ALIGN was not divisible by 2 \n");
      exit(1);
    }

    //align the buffer pointer  
    M_POINTER = (T*) malloc(ALIGN+M_NELM*sizeof(T));
    if (M_POINTER != NULL)
    {
      long M = (long)M_POINTER%(long)ALIGN; //number of bytes off
      if (M != 0)
      {
        //shift M_BUFFER start to match boundary
        char* cptr = (char*) M_POINTER;
        for (long i=0;i<((long)ALIGN-M);i++)
        {
          ++cptr;
        }
        M_BUFFER = (T*) cptr;
      } else {
        M_BUFFER = M_POINTER;
      }
    }

    //check all went well
    if (M_BUFFER == NULL || M_POINTER == NULL)
    {
      printf("ERROR libj::tensor::m_aligned_allocate\n");
      printf("could not allocate M_BUFFER \n");
      exit(1);
    }
    M_IS_ALLOCATED = true;
    M_ALIGNMENT = calc_alignment(M_BUFFER);

    //determine if memory is sequential
    m_sequential();

  } else {
    printf("ERROR libj::tensor::m_aligned_allocate\n");
    printf("attempted to allocate an already set vector\n");
    exit(1);
  }
}

//-----------------------------------------------------------------------
// determine if data is sequential 
//-----------------------------------------------------------------------
template <typename T> 
void tensor<T>::m_sequential()
{
  M_IS_SEQUENTIAL = true;
  size_t NN = 1;
  for (size_t dim=0;dim<M_NDIM;dim++)
  {
//    printf("dimension %zu, stride = %zu, NN = %zu, length = %zu \n",
//           dim,M_STRIDE[dim],NN,M_LENGTHS[dim]);
    if (NN != M_STRIDE[dim]) {M_IS_SEQUENTIAL = false; return;}
    NN *= M_LENGTHS[dim];
  }
}

//-----------------------------------------------------------------------
// allocate
//-----------------------------------------------------------------------
template <typename T> template<class...Rest>
void tensor<T>::allocate(const size_t first,const Rest...rest)
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    M_LENGTHS.push_back(first);
    M_STRIDE.push_back(1);
    M_NELM = first;
    m_init(rest...);
    m_allocate();
  } else {
    printf("ERROR libj::tensor::allocate\n");
    printf("attempted to allocated an already allocated tensor\n");
    exit(1);
  }
}

//-----------------------------------------------------------------------
// aligned allocate
//-----------------------------------------------------------------------
template <typename T> template<class...Rest>
void tensor<T>::aligned_allocate(const size_t BYTES, const size_t first,
                                 const Rest...rest)
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    M_LENGTHS.push_back(first);
    M_STRIDE.push_back(1);
    M_NELM = first;
    m_init(rest...);
    m_aligned_allocate(BYTES);
  } else {
    printf("ERROR libj::tensor::aligned_allocate\n");
    printf("attempted to allocated an already allocated tensor\n");
    exit(1);
  }
}

//-----------------------------------------------------------------------
// assign 
//-----------------------------------------------------------------------
template <typename T> template<class...Rest>
void tensor<T>::assign(T* pointer, const size_t first,const Rest...rest)
{
  if (!M_IS_ALLOCATED)
  {
    M_LENGTHS.push_back(first);
    M_STRIDE.push_back(1);
    M_NELM = first;
    m_init(rest...);
    m_assign(pointer);
  } else {
    printf("ERROR libj::tensor::assign\n");
    printf("attempted to assign an already allocated tensor\n");
    exit(1);
  }
}

//-----------------------------------------------------------------------
// deallocate via free 
//-----------------------------------------------------------------------
template<typename T>
void tensor<T>::deallocate()
{
  if (M_IS_ALLOCATED)
  {
    if (M_POINTER != NULL) {free(M_POINTER);}
    M_BUFFER = NULL;
  } else {
    printf("ERROR libj::tensor::deallocate \n");
    printf("attempted to deallocate an unallocated tensor \n");
    exit(1);
  }
} 

//-----------------------------------------------------------------------
// unassign
//-----------------------------------------------------------------------
template<typename T>
void tensor<T>::unassign()
{
  if (M_IS_ASSIGNED)
  {
    M_POINTER = NULL; 
    M_BUFFER = NULL;
    m_set_default(); 
  } else {
    printf("ERROR libj::tensor::unassign \n");
    printf("attempted to unassign an unassigned tensor \n");
    exit(1);
  }
} 

//-----------------------------------------------------------------------
// equals assignment 
//-----------------------------------------------------------------------
template <typename T>
void tensor<T>::operator= (const tensor<T>& other)
{
  //First, make sure this tensor is not allocated and the other is set 
  if (is_allocated())
  {
    printf("ERROR libj::tensor::= \n");
    printf("LHS tensor is already allcoated \n");
    exit(1);
  }
  if (!other.is_set())
  {
    printf("ERROR libj::tensor::= \n");
    printf("RHS tensor is not set \n");
    exit(1);
  }
 
  m_set_default();

  //Copy dimensions 
  M_NDIM = other.M_NDIM;
  M_NELM = other.M_NELM;
  for (size_t dim=0;dim<M_NDIM;dim++)
  {
    M_LENGTHS.push_back(other.M_LENGTHS[dim]); 
    M_STRIDE.push_back(other.M_STRIDE[dim]); 
  }
   
  //assign to the buffer of the other
  m_assign(other.M_BUFFER);

}

//-----------------------------------------------------------------------
// equals constructor
//-----------------------------------------------------------------------
template <typename T>
tensor<T>::tensor(const tensor<T>& other)
{
  //First, make sure this tensor is not allocated and the other is set 
  if (!other.is_set())
  {
    printf("ERROR libj::tensor::equals_constructor \n");
    printf("RHS tensor is not set \n");
    exit(1);
  }
 
  m_set_default();

  //Copy dimensions 
  M_NDIM = other.M_NDIM;
  M_NELM = other.M_NELM;
  for (size_t dim=0;dim<M_NDIM;dim++)
  {
    M_LENGTHS.push_back(other.M_LENGTHS[dim]); 
    M_STRIDE.push_back(other.M_STRIDE[dim]); 
  }
   
  //assign to the buffer of the other
  m_assign(other.M_BUFFER);

}

//-----------------------------------------------------------------------
// block
//	varadic template for creating a new tensor that points to a 
//	block of this tensor
//
// 	The first NDIM elements are the starting indices
//	  the next NDIM elements are the lengths of the blocks
//-----------------------------------------------------------------------
template <typename T> template<class...Rest>
tensor<T> tensor<T>::block(const size_t first, const Rest...rest) const
{
  //initialize the new tensor
  tensor<T> TNEW; 
  TNEW.M_NDIM = M_NDIM;
  TNEW.M_LENGTHS.reserve(M_NDIM);
  TNEW.M_STRIDE.reserve(M_NDIM);
  std::vector<size_t> start(M_NDIM);
  std::vector<size_t> length(M_NDIM);

  //initialize the start and length vectors
  start[0] = first;
  m_block(1,start,length,rest...);

  //Set the new tensor's shape -- make sure we don't go out of bounds!!
  TNEW.M_NELM = 1;
  T* pointer=M_BUFFER; 
  for (size_t dim=0;dim<M_NDIM;dim++)
  {
    TNEW.M_LENGTHS.push_back(std::min(length[dim],M_LENGTHS[dim] - start[dim]));  
    TNEW.M_STRIDE.push_back(M_STRIDE[dim]);
    TNEW.M_NELM *= TNEW.M_LENGTHS[dim];
    pointer += M_STRIDE[dim]*start[dim];
  }
  if (TNEW.M_NELM <= 0) 
  {
    printf("ERROR libj::tensor::block \n");
    printf("Tensor block had <= 0 elements \n");
    exit(1);
  }
  TNEW.m_assign(pointer);

  return TNEW;

}

//-----------------------------------------------------------------------
// m_block
//	internal function to construct block of matrix
//-----------------------------------------------------------------------
template<typename T>
void tensor<T>::m_block(const size_t level, std::vector<size_t>& start,
                        std::vector<size_t>& length, const size_t last) const
{
  length[level-M_NDIM] = last;
}

template <typename T> template<class...Rest>
void tensor<T>::m_block(const size_t level, std::vector<size_t>& start,
                        std::vector<size_t>& length, const size_t first,
                        const Rest...rest) const
{
  level < M_NDIM ? start[level] = first : length[level-M_NDIM] = first;
  m_block(level+1,start,length,rest...);
}

}//end of namespace


#endif
