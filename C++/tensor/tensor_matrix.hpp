/*----------------------------------------------------------------------------
  tensor_matrix.hpp
	JHT, April 25, 2022 : created

  .hpp file for the tensor_matrix class, which is used to "matrixicize" 
  a tensor. This is purely used to represent an underlying tensor, and
  can only be copied or assigned from a tensor.

  The LHS and RHS bundles are represented as a string, such as "adc","b"
  which uses "a" as a reference to the first dimension, "b" as the second
  dimension, etc etc. 

  Thus, a tensor with dimensions {4,1,3,2} coule be represented as a matrix
  with {4,3} and {1,2} index bundles if the LHS bundles are given as "ac" and
  the RHS bundles are given as "bd".

  NOTE: In the case of empty LHS and RHS bundles, the tensor is just taken to be
  a column vector

  Construction
  ---------------------
  libj::tensor_matrix<double> A;
  A.assign(tensor,"abc","d"); 			
  libj::tensor_matrix<int> B(tensor,"","");	//yields a col-vector rep. of tensor
 
  libj::tensor

  Matrix-style access
  ---------------------
  A(I,J); 

  Creating a subblock
  ---------------------
  libj::tensor_matrix<double> T2 = T1.block(ROW,COL,NUM_ROWS,NUM_COLS); 

  Useful functions
  ---------------------
  A.data();		//returns pointer to data buffer
  A.offset(I,J); 	//returns of offset from data buffer 
				for bundled indicies

----------------------------------------------------------------------------*/
#ifndef TENSOR_MATRIX_HPP
#define TENSOR_MATRIX_HPP

#include "tensor.hpp"
#include "index_bundle.hpp"
#include "alignment.hpp"
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>

namespace libj
{

//------------------------------------------------------------------------
// tensor_matrix class
//------------------------------------------------------------------------
template <typename T>
class tensor_matrix
{
  private:
  //data
  T*                   M_BUFFER;     //pointer to base data, offset in case of block
  libj::tensor<T>      M_TENSOR;     //pointer to tensor this represents
  libj::index_bundle   M_LHS;	     //left hand side index bundles
  libj::index_bundle   M_RHS;        //right hand size index bundles
  std::vector<size_t>  M_DUMMY;	     //dummy vector for access to M_TENSOR 

  //Internal functions
  void m_set_default(); //set the default values
  void m_set_dimensions(const std::string& lhs, const std::string& rhs);
  bool m_good_bundles();

  public:
  //Constructors
  tensor_matrix();

  //Copy constructor from tensor
  tensor_matrix(const libj::tensor<T>& tens, const std::string& lhs, const std::string& rhs);

  //Assignment
  void assign(libj::tensor<T>& tens, const std::string& lhs, const std::string& rhs);
  void assign(const libj::tensor<T>& tens, const std::string& lhs, const std::string& rhs);

  //getters
  size_t size() const {return M_LHS.NELM * M_RHS.NELM;} 
  size_t size(const size_t dim) const 
  {
    if (dim == 0) 
    {
      return M_LHS.NELM;
    } else {
      return M_RHS.NELM;
    }
  }

  //access functions
  T& operator() (const size_t I, const size_t J);
  const T& operator() (const size_t I, const size_t J) const;

  //offset function
  const size_t offset(const size_t I, const size_t J) const;

  //data function
  T* data() {return M_BUFFER;}
  const T* data() const {return M_BUFFER;} 

  //block function
  tensor_matrix<T> block(const size_t ROW, const size_t COL, 
                         const size_t NROW, const size_t NCOL);
  const tensor_matrix<T> block(const size_t ROW, const size_t COL, 
                               const size_t NROW, const size_t NCOL) const;

};//end of class

//-----------------------------------------------------------------------------------------
// set default values
//-----------------------------------------------------------------------------------------
template<typename T>
void tensor_matrix<T>::m_set_default()
{
  //don't set the tensor yet
  M_LHS.clear();
  M_RHS.clear();
  M_DUMMY.clear();
  M_BUFFER = NULL;
}

//-----------------------------------------------------------------------------------------
// empty constructor 
//-----------------------------------------------------------------------------------------
template <typename T>
tensor_matrix<T>::tensor_matrix()
{
  m_set_default();
}

//-----------------------------------------------------------------------------------------
// copy constructor 
//-----------------------------------------------------------------------------------------
template <typename T>
tensor_matrix<T>::tensor_matrix(const libj::tensor<T>& tens, const std::string& lhs,
                                const std::string& rhs)
{
  m_set_default();
  assign(tens,lhs,rhs);
}

//-----------------------------------------------------------------------------------------
// m_set_dimensions
//	sets the bundles and dimensions of the tensor_matrix
//-----------------------------------------------------------------------------------------
template<typename T>
void tensor_matrix<T>::m_set_dimensions(const std::string& lhs, 
                                        const std::string& rhs)
{
  //if both lhs and rhs are empty, make lhs and rhs such that this is a col vector
  if (lhs.length() == 0 && rhs.length()==0)
  {
    std::string new_lhs;
    new_lhs.reserve(M_TENSOR.dim());
    for (size_t i=0;i<M_TENSOR.dim();i++)
    {
      new_lhs.push_back((char)((int) 'a' + (int)i));
    }
    M_LHS.make_bundle<T>(M_TENSOR,new_lhs);
    M_RHS.make_bundle<T>(M_TENSOR,"");
  } else {
  
    //go through each and make the bundles 
    M_LHS.make_bundle<T>(M_TENSOR,lhs);
    M_RHS.make_bundle<T>(M_TENSOR,rhs);
  }

  //check if the bundles were good
  if (!m_good_bundles())
  { 
    printf("ERROR libj::tensor_matrix::m_set_dimensions \n");
    printf("The input bundles are bad \n");
    printf("LHS = %s \n",lhs.c_str());
    printf("RHS = %s \n",rhs.c_str());
    exit(1);
  }

  M_DUMMY.resize(M_TENSOR.dim());
}

//-----------------------------------------------------------------------------------------
// m_check_bundles
//-----------------------------------------------------------------------------------------
template <typename T>
bool tensor_matrix<T>::m_good_bundles()
{
  //check that the number of dimensions in each sums to tensor dimensions
  if (M_LHS.NDIM + M_RHS.NDIM != M_TENSOR.dim()) {return false;}

  //check that each dimension only appears once
  const size_t TOT = M_LHS.NDIM + M_RHS.NDIM;
  std::vector<size_t> COUNT(TOT);
  for (size_t dim = 0; dim < TOT ; dim++) {COUNT[dim] = 0;} 
  for (size_t idx = 0; idx < M_LHS.NDIM; idx++) {COUNT.at(M_LHS.DIM[idx])++;}
  for (size_t idx = 0; idx < M_RHS.NDIM; idx++) {COUNT.at(M_RHS.DIM[idx])++;}
  for (size_t dim = 0; dim < TOT; dim++) {if (COUNT[dim] != 1) {return false;}}
  return true;
}


//-----------------------------------------------------------------------------------------
// Assigment 
//-----------------------------------------------------------------------------------------
template<typename T>
void tensor_matrix<T>::assign(libj::tensor<T>& tens, 
                             const std::string& lhs, const std::string& rhs)
{
  //reset default values (incase the tensor_matrix is already set)
  m_set_default();

  //assign the pointer
  M_TENSOR = tens;
  M_BUFFER = M_TENSOR.data(); 

  //set the dimensions
  m_set_dimensions(lhs,rhs);
}

template<typename T>
void tensor_matrix<T>::assign(const libj::tensor<T>& tens, 
                              const std::string& lhs, const std::string& rhs)
{
  //reset default values (incase the tensor_matrix is already set)
  m_set_default();

  //assign the pointer
  M_TENSOR = tens;
  M_BUFFER = M_TENSOR.data();

  //set the dimensions
  m_set_dimensions(lhs,rhs);
}

//-----------------------------------------------------------------------------------------
// Access operators
//-----------------------------------------------------------------------------------------
template <typename T>
T& tensor_matrix<T>::operator() (const size_t I, const size_t J)
{
  //LHS
  for (size_t idx = 0; idx < M_LHS.NDIM; idx++)
  {
    const size_t dim = M_LHS.DIM[idx];
    M_DUMMY[dim] = M_LHS.get_index(I,idx);
  }

  //RHS
  for (size_t idx = 0; idx < M_RHS.NDIM; idx++)
  {
    const size_t dim = M_RHS.DIM[idx];
    M_DUMMY[dim] = M_RHS.get_index(J,idx);
  }
 
  return M_TENSOR(M_DUMMY);
}

template <typename T>
const T& tensor_matrix<T>::operator() (const size_t I, const size_t J) const
{
  std::vector<size_t> vec(M_LHS.NDIM + M_RHS.NDIM);
  //LHS
  for (size_t idx = 0; idx < M_LHS.NDIM; idx++)
  {
    const size_t dim = M_LHS.DIM[idx];
    vec[dim] = M_LHS.get_index(I,idx);
  }

  //RHS
  for (size_t idx = 0; idx < M_RHS.NDIM; idx++)
  {
    const size_t dim = M_RHS.DIM[idx];
    vec[dim] = M_RHS.get_index(J,idx);
  }

  return M_TENSOR(vec);
}

//-----------------------------------------------------------------------------------------
// offset
//	returns offset for bundle indices in the original matrix
//-----------------------------------------------------------------------------------------
template <typename T>
const size_t tensor_matrix<T>::offset(const size_t I, const size_t J) const
{
  std::vector<size_t> vec(M_LHS.NDIM + M_RHS.NDIM);

  //LHS
  for (size_t idx = 0; idx < M_LHS.NDIM; idx++)
  {
    const size_t dim = M_LHS.DIM[idx];
    vec[dim] = M_LHS.get_index(I,idx);
  }

  //RHS
  for (size_t idx = 0; idx < M_RHS.NDIM; idx++)
  {
    const size_t dim = M_RHS.DIM[idx];
    vec[dim] = M_RHS.get_index(J,idx);
  }

  return M_TENSOR.offset(vec); 

}

//-----------------------------------------------------------------------------------------
// block 
//	Note that this block really comes down to altering the index bundles, as they track
//	the actual block position within the original tensor
//-----------------------------------------------------------------------------------------
template <typename T>
tensor_matrix<T> tensor_matrix<T>::block(const size_t ROW, const size_t COL,
                                         const size_t NROW, const size_t NCOL)
{
  tensor_matrix<T> TENS;
  TENS.M_TENSOR = M_TENSOR; //same source tensor
  TENS.M_LHS = M_LHS.block(ROW,NROW); //LHS bundles change
  TENS.M_RHS = M_RHS.block(COL,NCOL); //RHS bundles change
  TENS.M_DUMMY.resize(M_LHS.NDIM + M_RHS.NDIM);
  TENS.M_BUFFER = &(*this)(ROW,COL); 
  return TENS; 
}

template <typename T>
const tensor_matrix<T> tensor_matrix<T>::block(const size_t ROW, const size_t COL,
                                               const size_t NROW, const size_t NCOL) const
{
  tensor_matrix<T> TENS;
  TENS.M_TENSOR = M_TENSOR; //same source tensor
  TENS.M_LHS = M_LHS.block(ROW,NROW); //LHS bundles change
  TENS.M_RHS = M_RHS.block(COL,NCOL); //RHS bundles change
  TENS.M_DUMMY.resize(M_LHS.NDIM + M_RHS.NDIM);
  TENS.M_BUFFER = &(*this)(ROW,COL); 
  return TENS; 
}


}//end of namespace

#endif
