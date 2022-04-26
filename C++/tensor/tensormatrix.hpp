/*----------------------------------------------------------------------------
  tensormatrix.hpp
	JHT, April 25, 2022 : created

  .hpp file for the tensormatrix class, which is used to "matrixicize" 
  a tensor. This is purely used to represent an underlying tensor, and
  thus can *only* be assigned

  Note that row-scatter and col-scatter vectors are not contructed here

  Construction
  ---------------------
  libj::tensormatrix<double> A;
  libj::tensor<double> B(1,3,2,5);
  A.assign(tensor.block(0,0,0,0,1,2,2,1)),"acd","b");

  Matrix-style access
  ---------------------
  A(I,J); 

----------------------------------------------------------------------------*/
#ifndef TENSORMATRIX_HPP
#define TENSORMATRIX_HPP

#include "tensor.hpp"
#include "alignment.hpp"
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>

namespace libj
{

//------------------------------------------------------------------------
// index bundle class
//------------------------------------------------------------------------
struct index_bundle
{
  size_t              NDIM;   //number of dimensions
  size_t              NELM;   //number of elements
  std::vector<size_t> DIM;    //dimension list
  std::vector<size_t> STRIDE; //stride list
  std::vector<size_t> LENGTH; //length list

  //initialize the bundle to nothing
  index_bundle()
  {
    clear();
  }
  
  //clear the data in the bundle
  void clear()
  {
    NDIM = 0;
    NELM = 0;
    DIM.clear();
    STRIDE.clear();
    LENGTH.clear();
  }
  
  size_t size() const {return NELM;}
 
  //given cumulative index I, return the value for sub-index idx
  size_t get_index(const size_t I, const size_t idx)
  {
    return (I/STRIDE[idx])%LENGTH[idx];
  } 

  //convert char to dimension number
  size_t c2dim(const char c)
  {
    return (size_t) (std::tolower(c) - (int) 'a');
  }

  //make the bundle
  template<typename T>
  void make_bundle(libj::tensor<T>* tens, const std::string& str)
  {
    NDIM = str.length();

    DIM.resize(NDIM);
    LENGTH.resize(NDIM);
    STRIDE.resize(NDIM);
    
    NELM = 1;
    for (size_t idx=0;idx<NDIM;idx++)
    {
      const size_t dim = c2dim((str[idx])); 
      DIM[idx] = dim;
      LENGTH[idx] = tens->size(dim);
      idx > 0 ? STRIDE[idx] = STRIDE[idx-1]*LENGTH[idx-1] : STRIDE[0] = 1;  
      NELM *= LENGTH[idx];  
    }
  }
};

//------------------------------------------------------------------------
// tensormatrix class
//------------------------------------------------------------------------
template <typename T>
class tensormatrix
{
  private:
  //data
  libj::tensor<T>*     M_TENSOR;     //pointer to tensor this represents
  libj::index_bundle   M_LHS;	     //left hand side index bundles
  libj::index_bundle   M_RHS;        //right hand size index bundles
  std::vector<size_t>  M_DUMMY;	     //dummy vector for access to M_TENSOR 

  //Internal functions
  void m_set_default(); //set the default values
  void m_set_dimensions(const std::string& lhs, const std::string& rhs);
  bool m_good_bundles();

  public:
  //Constructors
  tensormatrix();

  //Assignment
  void assign(libj::tensor<T>& tens, const std::string& lhs, const std::string& rhs);

  //getters
  size_t size() const {return M_TENSOR->size();} 
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

  //Block function

};//end of class

//-----------------------------------------------------------------------------------------
// set default values
//-----------------------------------------------------------------------------------------
template<typename T>
void tensormatrix<T>::m_set_default()
{
  M_TENSOR = NULL;
  M_LHS.clear();
  M_RHS.clear();
  M_DUMMY.clear();
}

//-----------------------------------------------------------------------------------------
// empty constructor 
//-----------------------------------------------------------------------------------------
template <typename T>
tensormatrix<T>::tensormatrix()
{
  m_set_default();
}

//-----------------------------------------------------------------------------------------
// m_set_dimensions
//	sets the bundles and dimensions of the tensormatrix
//-----------------------------------------------------------------------------------------
template<typename T>
void tensormatrix<T>::m_set_dimensions(const std::string& lhs, 
                                       const std::string& rhs)
{
  //go through each and make the bundles 
  M_LHS.make_bundle<T>(M_TENSOR,lhs);
  M_RHS.make_bundle<T>(M_TENSOR,rhs);


  //check if the bundles were good
  if (!m_good_bundles())
  { 
    printf("ERROR libj::tensormatrix::m_set_dimensions \n");
    printf("The input bundles are bad \n");
    printf("LHS = %s \n",lhs.c_str());
    printf("RHS = %s \n",rhs.c_str());
    exit(1);
  }

  M_DUMMY.resize(M_TENSOR->dim());
}

//-----------------------------------------------------------------------------------------
// m_check_bundles
//-----------------------------------------------------------------------------------------
template <typename T>
bool tensormatrix<T>::m_good_bundles()
{
  //check that the number of dimensions in each sums to tensor dimensions
  if (M_LHS.NDIM + M_RHS.NDIM != M_TENSOR->dim()) {return false;}

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
void tensormatrix<T>::assign(libj::tensor<T>& tens, 
                             const std::string& lhs, const std::string& rhs)
{
  //reset default values (incase the tensormatrix is already set)
  printf("before set default\n");
  m_set_default();

  //assign the pointer
  printf("before assignment \n"); 
  M_TENSOR = &tens;
  printf("location of &tens %p \n",(void*) &tens);
  printf("location of M_TENSOR %p \n",(void*) M_TENSOR);

  //set the dimensions
  printf("before set dimensions \n"); 
  m_set_dimensions(lhs,rhs);
  printf("after set dimensions \n"); 
  
}

//-----------------------------------------------------------------------------------------
// Access operators
//-----------------------------------------------------------------------------------------
template <typename T>
T& tensormatrix<T>::operator() (const size_t I, const size_t J)
{
  printf("LHS nelm %zu ",M_LHS.size());
  //LHS
  for (size_t idx = 0; idx < M_LHS.NDIM; idx++)
  {
    const size_t dim = M_LHS.DIM[idx];
    M_DUMMY[dim] = M_LHS.get_index(I,idx);
  }

  //RHS
  printf("LHS nelm %zu ",M_RHS.size());
  for (size_t idx = 0; idx < M_RHS.NDIM; idx++)
  {
    const size_t dim = M_RHS.DIM[idx];
    M_DUMMY[dim] = M_RHS.get_index(J,idx);
  }
 
  printf("M_DUMMY = %zu ",M_DUMMY[0]); 
  for (size_t dim=1;dim<M_LHS.NDIM + M_RHS.NDIM;dim++)
  {
    printf("%zu ",M_DUMMY[dim]);
  }

  return (*M_TENSOR)(M_DUMMY);
}

template <typename T>
const T& tensormatrix<T>::operator() (const size_t I, const size_t J) const
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

  return (*M_TENSOR)(vec);
}
}//end of namespace

#endif
