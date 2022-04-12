/*----------------------------------------------------------------------------
  tensor.hpp
	JHT, April 10, 2022 : created

  .hpp file for the general tensor class. This behaves similarly to 
  std::array in that it cannot be grown dynamically. The default 
  storage is column-major.

  For good performance, you really want to call this via the [] operator
    and have an external fucntion determine the index from the offsets

  INITIALIZATION
  -------------------
  Create, but do not assign or allocate
    libj::tensor<double> T; 

  Create and allocate with set legnths via malloc
    libj::tensor<double>({1,4,3});

  Create and assign to memory with set lengths
    libj::tensor<double>({1,4,3},pointer);

  Allocate or assign an existing tensor:
    T.allocate({1,4,3});
    T.assign({1,4,3},pointer);

  ELEMENT ACCESS
  ------------------
  To access the n'th element
    T[N];

  To use the index access
    T({1,2,3});
  
----------------------------------------------------------------------------*/
#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <stdio.h>
#include <vector>
#include "alignment.hpp"

namespace libj 
{

template<typename T>
class tensor
{
  private:
  T*    M_BUFFER;       //start of data
  int*  M_LENGTHS;      //list of dimension lengths
  long* M_OFFSETS;	//list of offsets for data access
  int   M_NUM_DIM;      //number of dimensions
  long  M_NUM_ELM;      //total number of elements
  int   M_ALIGNMENT;    //alignment in bytes
  bool  M_IS_ALLOCATED; //tensor is allocated with malloc 
  bool  M_IS_ASSIGNED;  //tensor is assigned with malloc 

  //setting functions
  void set_dim(const std::vector<int> lengths);
  void unset_dim();
  void set_default();

  public: 
  //Initialize and destroy
  tensor();
  tensor(const std::vector<int> lengths);
  tensor(const std::vector<int> lengths, T* ptr);
 ~tensor();

  //Allocation or assignment
  void allocate(const std::vector<int> lengths);
  void assign(const std::vector<int> lengths, T* pointer);
  void deallocate();
  void unassign();
  void reassign(const std::vector<int> lengths, T* pointer);
  
  //Getters
  const long size() const {return M_NUM_ELM;}
  const int size(const int dim) const {return M_LENGTHS[dim];}
  const int dim() const {return M_NUM_DIM;}
  const int alignment() const {return M_ALIGNMENT;}
  const long offset(const int dim) const {return M_OFFSETS[dim];}
  const bool is_allocated() const {return M_IS_ALLOCATED;}
  const bool is_assigned() const {return M_IS_ASSIGNED;}
  const bool is_set() const {return M_IS_ALLOCATED || M_IS_ASSIGNED;}

  //element access
  T& operator() (const std::vector<int> index)
  {
    int I;
    int IDX=index[0]; 
    for (I=1;I<M_NUM_DIM;I++) {IDX += index[I]*M_OFFSETS[I];}
    return *(M_BUFFER+IDX);
  }

  const T& operator() (const std::vector<int> index) const
  {
    int I;
    int IDX=index[0]; 
    for (I=1;I<M_NUM_DIM;I++) {IDX += index[I]*M_OFFSETS[I];} 
    return *(M_BUFFER+IDX);
  }
  
  T& operator[] (const long offset) {return *(M_BUFFER+offset);}
  const T& operator[] (const long offset) const {return *(M_BUFFER+offset);}

}; //end of normal tensor

template class tensor<double>;
template class tensor<long>;
template class tensor<float>;
template class tensor<int>;

template <typename T>
bool same_shape(const tensor<T>& A, const tensor<T>& B)
{
  if (A.dim() != B.dim()) {return false;}
  for (int dim=0;dim<A.dim();dim++)
  {
    if (A.size(dim) != B.size(dim)) {return false;}
  }
  return true;
}
template bool same_shape<double>(const tensor<double>& A, const tensor<double>& B);
template bool same_shape<float>(const tensor<float>& A, const tensor<float>& B);
template bool same_shape<long>(const tensor<long>& A, const tensor<long>& B);
template bool same_shape<int>(const tensor<int>& A, const tensor<int>& B);

}//end of namespace

#endif
