/*----------------------------------------------------------------------------
  tensor.cpp
	JHT, April 10, 2022 : created

  .cpp for the general tensor class
----------------------------------------------------------------------------*/
#include "tensor.hpp"

//----------------------------------------------------------------------------
// set_default
//	sets the default data for the tensor initialization
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::set_default()
{
  M_BUFFER = NULL;
  M_LENGTHS = NULL;
  M_OFFSETS = NULL;
  M_NUM_DIM = 0;
  M_NUM_ELM = 0; 
  M_ALIGNMENT = 0;
  M_IS_ALLOCATED = false;
  M_IS_ASSIGNED = false;
}
template void libj::tensor<double>::set_default();
template void libj::tensor<float>::set_default();
template void libj::tensor<long>::set_default();
template void libj::tensor<int>::set_default();

//----------------------------------------------------------------------------
// set_dim
// 	sets the dimension data
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::set_dim(const std::vector<int> lengths)
{
  M_NUM_DIM = lengths.size();  

  //Allocate lengths array
  M_LENGTHS = (int*) malloc(sizeof(int)*M_NUM_DIM);
  M_OFFSETS = (long*) malloc(sizeof(long)*M_NUM_DIM);
  if (M_LENGTHS == NULL || M_OFFSETS == NULL)
  {
    printf("ERROR libj::tensor::set_dim could not malloc M_LENGTHS or M_OFFSETS\n");
    exit(1);
  }

  //set the dimension data 
  M_OFFSETS[0] = (long) 0;
  M_LENGTHS[0] = lengths[0];
  M_NUM_ELM = (long)M_LENGTHS[0]; 
  if (M_LENGTHS[0] == 0) 
  {
    printf("ERROR libj::tensor::set_dim\n");
    printf("tensor had a dimension with 0 length \n");
    exit(1);
  }
   
  for (int dim=1;dim<M_NUM_DIM;dim++) 
  {
    M_OFFSETS[dim] = M_NUM_ELM; 
    M_LENGTHS[dim] = lengths[dim];
    M_NUM_ELM *= (long)M_LENGTHS[dim];

    //check for bad elements
    if (M_LENGTHS[dim] == 0) 
    {
      printf("ERROR libj::tensor::set_dim\n");
      printf("tensor had a dimension with 0 length \n");
      exit(1);
    }

  }

}
template void libj::tensor<double>::set_dim(const std::vector<int> lengths);
template void libj::tensor<float>::set_dim(const std::vector<int> lengths);
template void libj::tensor<long>::set_dim(const std::vector<int> lengths);
template void libj::tensor<int>::set_dim(const std::vector<int> lengths);

//----------------------------------------------------------------------------
// unset_dim
//	resets the dimensions to zero
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::unset_dim()
{
  if (M_LENGTHS != NULL) {free(M_LENGTHS);} 
  if (M_OFFSETS != NULL) {free(M_OFFSETS);} 
  set_default();
}
template void libj::tensor<double>::unset_dim();
template void libj::tensor<float>::unset_dim();
template void libj::tensor<long>::unset_dim();
template void libj::tensor<int>::unset_dim();

//----------------------------------------------------------------------------
// Construct, but do not allocate or assign
//----------------------------------------------------------------------------
template<typename T>
libj::tensor<T>::tensor()
{
  set_default();
}
template libj::tensor<double>::tensor();
template libj::tensor<float>::tensor();
template libj::tensor<long>::tensor();
template libj::tensor<int>::tensor();

//----------------------------------------------------------------------------
// destructor 
//----------------------------------------------------------------------------
template<typename T>
libj::tensor<T>::~tensor()
{
 if (M_IS_ALLOCATED) {deallocate();} 
}
template libj::tensor<double>::~tensor();
template libj::tensor<float>::~tensor();
template libj::tensor<long>::~tensor();
template libj::tensor<int>::~tensor();


//----------------------------------------------------------------------------
// Construct with known dimensions, allocate via malloc
//----------------------------------------------------------------------------
template<typename T>
libj::tensor<T>::tensor(const std::vector<int> lengths)
{
  set_default();
  allocate(lengths);
}
template libj::tensor<double>::tensor(const std::vector<int> lengths);
template libj::tensor<float>::tensor(const std::vector<int> lengths);
template libj::tensor<long>::tensor(const std::vector<int> lengths);
template libj::tensor<int>::tensor(const std::vector<int> lengths);

//----------------------------------------------------------------------------
// Construct with known dimensions, assign to preallcoated memory
//----------------------------------------------------------------------------
template<typename T>
libj::tensor<T>::tensor(const std::vector<int> lengths, T* pointer)
{
  set_default();
  assign(lengths,pointer);
}
template libj::tensor<double>::tensor(const std::vector<int> lengths, double* pointer);
template libj::tensor<float>::tensor(const std::vector<int> lengths, float* pointer);
template libj::tensor<long>::tensor(const std::vector<int> lengths, long* pointer);
template libj::tensor<int>::tensor(const std::vector<int> lengths, int* pointer);

//----------------------------------------------------------------------------
// allocate the vector
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::allocate(const std::vector<int> lengths)
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    set_dim(lengths);
    M_BUFFER = (T*) malloc(sizeof(T)*M_NUM_ELM);
    if (M_BUFFER == NULL)   
    {
      printf("ERROR libj::tensor::allocate could not allocate M_BUFFER \n");
      exit(1);
    }
    M_IS_ALLOCATED = true;
    M_ALIGNMENT = calc_alignment(M_BUFFER);

  } else {
    printf("ERROR libj::tensor::allocate attempted to allocate an already set vector\n");
    exit(1);
  }
}
template void libj::tensor<double>::allocate(const std::vector<int> lengths);
template void libj::tensor<float>::allocate(const std::vector<int> lengths);
template void libj::tensor<long>::allocate(const std::vector<int> lengths);
template void libj::tensor<int>::allocate(const std::vector<int> lengths);

//----------------------------------------------------------------------------
// assign the vector 
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::assign(const std::vector<int> lengths, T* pointer)
{
  if (!M_IS_ALLOCATED && !M_IS_ASSIGNED)
  {
    set_dim(lengths);
    M_BUFFER = pointer;
    if (M_BUFFER == NULL)   
    {
      printf("ERROR libj::tensor::assign could not M_BUFFER points to NULL \n");
      exit(1);
    }
    M_IS_ASSIGNED = true;
    M_ALIGNMENT = calc_alignment(M_BUFFER);

  } else {
    printf("ERROR libj::tensor::assign attempted to assign an already set vector\n");
    exit(1);
  }
}
template void libj::tensor<double>::assign(const std::vector<int> lengths, double* pointer);
template void libj::tensor<float>::assign(const std::vector<int> lengths, float* pointer);
template void libj::tensor<long>::assign(const std::vector<int> lengths, long* pointer);
template void libj::tensor<int>::assign(const std::vector<int> lengths, int* pointer);

//----------------------------------------------------------------------------
// deallocate
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::deallocate()
{
  if (M_IS_ALLOCATED)
  {
    if (M_BUFFER != NULL) {free(M_BUFFER);}
    unset_dim();
  } else {
    printf("ERROR Attempted to deallocate an unallocated tensor \n");
    exit(1);
  }
}
template void libj::tensor<double>::deallocate();
template void libj::tensor<float>::deallocate();
template void libj::tensor<long>::deallocate();
template void libj::tensor<int>::deallocate();

//----------------------------------------------------------------------------
// unassign 
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::unassign()
{
  if (M_IS_ASSIGNED)
  {
    M_BUFFER = NULL; 
    unset_dim();
  } else {
    printf("ERROR Attempted to unassign an unallocated tensor \n");
    exit(1);
  }
}
template void libj::tensor<double>::unassign();
template void libj::tensor<float>::unassign();
template void libj::tensor<long>::unassign();
template void libj::tensor<int>::unassign();

//----------------------------------------------------------------------------
// reassign
//----------------------------------------------------------------------------
template<typename T>
void libj::tensor<T>::reassign(const std::vector<int> lengths, T* ptr)
{
  unassign();
  assign(lengths,ptr);
}
template void libj::tensor<double>::reassign(const std::vector<int> lengths, 
                                             double* ptr);
template void libj::tensor<float>::reassign(const std::vector<int> lengths, 
                                            float* ptr);
template void libj::tensor<long>::reassign(const std::vector<int> lengths, 
                                           long* ptr);
template void libj::tensor<int>::reassign(const std::vector<int> lengths, 
                                          int* ptr);
