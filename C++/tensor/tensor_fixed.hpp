/*----------------------------------------------------------------------------
  tensor.hpp
	JHT, April 10, 2022 : created

  .hpp file for the general tensor class. This behaves similarly to 
  std::array in that it cannot be grown dynamically, though it can be 
  allocated (similar to a fortran dimension). 

  The storage is column major

  Note that the index access via () is implemented via a varadic template, 
  and thus should be rather efficient, though [], which accesses based on the
  offset should still be prefered

  INITIALIZATION
  -------------------
  Create, but do not assign or allocate
    libj::tensor<double,3,3,3> T; 

  Create and allocate with set legnths via malloc
    libj::tensor<double,3> A(1,4,3);

  Create and assign to memory with set lengths
    libj::tensor<double,3> A(1,4,3,pointer);

  Allocate or assign an existing tensor:
    T.allocate(1,4,3);
    T.aligned_allocate(64,1,4,3); //where 64 is the byte alignment 
    T.assign(1,4,3,pointer);

  Deallocate
    T.deallocate();
    T.unassign();

  Reassignment (including reshaping)
    T.assign(2,5,1,new_pointer);
  

  ELEMENT ACCESS
  ------------------
  To access the n'th element
    T[N];

  To use the index access
    T(1,2,3);


  USEFUL FUNCTIONS
  --------------------
    same_shape<double>(T1,T2);	//returns true if T1 and T2 (double tensors) 
            			//  are the same shape, false otherwise
  
----------------------------------------------------------------------------*/
#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "alignment.hpp"

//Standard alignment
#define DEFAULT_ALIGN 16

//Macro for making index array from varadic list of ints
#define make_index(dims,i0)\
  dims[0] = i0; \
  va_list args; va_start(args,i0); \
  for (size_t i=1;i<N;i++) {dims[i] = va_arg(args,size_t);} \
  va_end(args);

namespace libj 
{

template<typename T, const size_t N>
class tensor
{
  private:
  T*       M_BUFFER;       //start of data
  T*       M_POINTER;	//pointer to malloc
  size_t   M_LENGTHS[N];   //list of dimension lengths
  size_t   M_OFFSETS[N];	//list of offsets for data access
  size_t   M_NUM_ELM;      //total number of elements
  size_t   M_ALIGNMENT;    //alignment in bytes
  bool     M_IS_ALLOCATED; //tensor is allocated with malloc 
  bool     M_IS_ASSIGNED;  //tensor is assigned with malloc 

  //internal functions
  void m_set_default();
  void m_set_dim(const size_t* dim);
  void m_allocate();
  void m_aligned_allocate(const size_t BYTES);
  void m_assign(T* pointer);

  //internal varadic templates for data access...fancy stuff
  template<class...Rest>
  size_t m_index(const size_t level, const size_t first, const Rest...rest) const
  {
    return first*M_OFFSETS[level] + m_index(level+1,rest...); 
  }
  size_t m_index(const size_t level, const size_t first) const
  {
    return first*M_OFFSETS[level];
  }
  
  
  public:

  //Constructors and destructors
  tensor();			
  tensor(const size_t i0,...);	
  tensor(T* pointer, const size_t i0,...);
  ~tensor();			

  //allocate,assign
  void deallocate();
  void allocate(const size_t i0,...);
  void aligned_allocate(const size_t BYTES, const size_t i0,...);
  void assign(T* pointer, const size_t i0,...);
  void unassign();

  //Getters
  const size_t  size() const {return M_NUM_ELM;}
  const size_t  size(const size_t dim) const {return M_LENGTHS[dim];}
  const size_t  dim() const {return N;}
  const size_t  alignment() const {return M_ALIGNMENT;}
  const size_t  offset(const size_t dim) const {return M_OFFSETS[dim];}
  const bool    is_allocated() const {return M_IS_ALLOCATED;}
  const bool    is_assigned() const {return M_IS_ASSIGNED;}
  const bool    is_set() const {return M_IS_ALLOCATED || M_IS_ASSIGNED;}

  //Access functions
  T& operator[] (const size_t offset) {return *(M_BUFFER+offset);}
  const T& operator[] (const size_t offset) const {return *(M_BUFFER+offset);}

  template<class...Rest>
  T& operator() (const size_t i0,const Rest...rest)
  {
    return *(M_BUFFER + m_index(1,rest...));
  }

  template<class...Rest>
  const T& operator() (const size_t i0,const Rest...rest) const
  {
    return *(M_BUFFER + m_index(1,rest...));
  }

}; //end of normal tensor

//-----------------------------------------------------------------------
// returns true if the tensors are the same shape
//-----------------------------------------------------------------------
template <typename T, const size_t N>
bool same_shape(const tensor<T,N>& A, const tensor<T,N>& C)
{
  if (A.dim() != C.dim()) {return false;}
  for (int dim=0;dim<A.dim();dim++)
  {
    if (A.size(dim) != C.size(dim)) {return false;}
  }
  return true;
}

//-----------------------------------------------------------------------
// Initialization functions
//-----------------------------------------------------------------------
template<typename T, const size_t N>
void tensor<T,N>::m_set_default()
{ 
  M_BUFFER = NULL;
  M_POINTER = NULL;
  for (int i=0;i<N;i++) {M_LENGTHS[i] = 0; M_OFFSETS[i]=0;}
  M_NUM_ELM = 0; 
  M_ALIGNMENT = 0; 
  M_IS_ALLOCATED = false;
  M_IS_ASSIGNED = false;
}

//-----------------------------------------------------------------------
// set_dim
//	sets the dimensions of the tensor
//-----------------------------------------------------------------------
template<typename T, const size_t N>
void tensor<T,N>::m_set_dim(const size_t* dim)
{
  //initialize the data
  M_OFFSETS[0] = (size_t) 0;
  M_LENGTHS[0] = dim[0];
  M_NUM_ELM    = (size_t) M_LENGTHS[0];
  if (M_LENGTHS[0] == 0)
  {
    printf("ERROR libj::tensor::set_dim\n");
    printf("tensor had dimension with 0 length \n");
    exit(1);
  }

  //set the data
  for (size_t i=1;i<N;i++)
  {
    M_OFFSETS[i] = M_NUM_ELM;
    M_LENGTHS[i] = dim[i];
    M_NUM_ELM *= M_LENGTHS[i]; 

    //check for bad elements
    if (M_LENGTHS[i] <= 0)
    {
      printf("ERROR libj::tensor::set_dim \n");
      printf("tensor had dimension with 0 length\n");
      exit(1);
    }
  }
}

//-----------------------------------------------------------------------
//Blank contructor
//-----------------------------------------------------------------------
template<typename T, const size_t N>
tensor<T,N>::tensor()
{
  m_set_default();
}

//-----------------------------------------------------------------------
// blank destructor
//-----------------------------------------------------------------------
template<typename T, const size_t N>
tensor<T,N>::~tensor()
{
  if (M_IS_ALLOCATED) deallocate();
}

//-----------------------------------------------------------------------
//Allocator constructor
//-----------------------------------------------------------------------
template<typename T, const size_t N>
tensor<T,N>::tensor(const size_t i0,...)
{
  //set the default
  m_set_default();

  //Make the index array
  size_t dim[N]; make_index(dim,i0);

  //set the dimensions
  m_set_dim(dim);

  //call the internal allocate function
  m_allocate();
}

//-----------------------------------------------------------------------
//Assignment constructor
//-----------------------------------------------------------------------
template<typename T, const size_t N>
tensor<T,N>::tensor(T* pointer, const size_t i0,...)
{
  //set the default
  m_set_default();

  //Make the index array
  size_t dim[N]; make_index(dim,i0);

  //set the dimensions
  m_set_dim(dim);

  //call the internal allocate function
  m_assign(pointer);
}

//-----------------------------------------------------------------------
// internal malloc
//-----------------------------------------------------------------------
template<typename T, const size_t N>
void tensor<T,N>::m_allocate()
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    M_POINTER = (T*) malloc(sizeof(T)*M_NUM_ELM);
    M_BUFFER = M_POINTER;
    if (M_BUFFER == NULL || M_POINTER == NULL)
    {
      printf("ERROR libj::tensor::allocate could not allocate M_BUFFER \n");
      exit(1);
     }
     M_IS_ALLOCATED = true;
     M_ALIGNMENT = calc_alignment(M_BUFFER);

   } else {
     printf("ERROR libj::tensor::m_allocate\n");
     printf("attempted to allocate an already set vector\n");
     exit(1);
   }
}

//-----------------------------------------------------------------------
// internal assign
//-----------------------------------------------------------------------
template<typename T, const size_t N>
void tensor<T,N>::m_assign(T* pointer)
{
  if (!M_IS_ALLOCATED)
  {
    M_BUFFER = pointer;
    M_POINTER = pointer;
    M_IS_ASSIGNED = true;
    M_ALIGNMENT = calc_alignemnt(M_BUFFER);
  } else {
    printf("ERROR libj::tensor::m_assign\n");
    printf("attempted to assign an already allocated vector\n");
    exit(1);
   }
}

//----------------------------------------------------------------------------
// internal aligned_allocate 
//----------------------------------------------------------------------------
template<typename T, const size_t N>
void libj::tensor<T,N>::m_aligned_allocate(const size_t ALIGN)
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
    M_POINTER = (T*) malloc(ALIGN+M_NUM_ELM*sizeof(T));
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

  } else {
    printf("ERROR libj::tensor::m_aligned_allocate\n");
    printf("attempted to allocate an already set vector\n");
    exit(1);
  }
}

//-----------------------------------------------------------------------
// allocate
//-----------------------------------------------------------------------
template <typename T, const size_t N>
void tensor<T,N>::allocate(const size_t i0,...)
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    size_t dims[N];make_index(dims,i0);
    m_set_dim(dims);    
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
template <typename T, const size_t N>
void tensor<T,N>::aligned_allocate(const size_t BYTES, const size_t i0,...)
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    size_t dims[N];make_index(dims,i0);
    m_set_dim(dims);    
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
template <typename T, const size_t N>
void tensor<T,N>::assign(T* pointer, const size_t i0,...)
{
  if (!M_IS_ALLOCATED)
  {
    size_t dims[N];make_index(dims,i0);
    m_set_dim(dims);    
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
template<typename T, const size_t N>
void tensor<T,N>::deallocate()
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
template<typename T, const size_t N>
void tensor<T,N>::unassign()
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

}//end of namespace

#endif
