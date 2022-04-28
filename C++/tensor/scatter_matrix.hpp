/*----------------------------------------------------------------------------
  scatter_matrix.hpp
	JHT, April 25, 2022 : created

  .hpp file for the scatter_matrix class, which is used to access a 
  tensor_matrix in an out-of-order fashion. 

  This is constructed from a tensor_matrix, which has already performed the
  matrixification proccess. 

  NOTE : we const_cast the buffer pointer from the references tensor_matrix,
         be super careful that you are calling const when you need to be...

----------------------------------------------------------------------------*/
#ifndef SCATTER_MATRIX_HPP
#define SCATTER_MATRIX_HPP

#include "tensor.hpp"
#include "tensor_matrix.hpp"
#include "index_bundle.hpp"
#include "alignment.hpp"
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>

namespace libj
{

//------------------------------------------------------------------------
// scatter_matrix class
//------------------------------------------------------------------------
template <typename T>
class scatter_matrix
{
  private:
  //data
  T*                     M_BUFFER;	//base pointer
  std::vector<size_t>    M_RSCAT;       //row scatter vector
  std::vector<size_t>    M_CSCAT;       //col scatter vector
  size_t                 M_NELM;        //number of total elements
  size_t                 M_NROW;        //number of rows 
  size_t                 M_NCOL;        //number of cols 

  //internal functions
  void m_set_default();
  void m_set_scatter(const libj::tensor_matrix<T>& TMAT);
  
  public:

  //initialization
  scatter_matrix();
  
  //copy constructor
  scatter_matrix(const libj::tensor_matrix<T>& TMAT);

  //copy assignment constructor
  scatter_matrix& operator= (const libj::tensor_matrix<T>& TMAT);

  //Access operator
  T& operator() (const size_t I, const size_t J) 
  {
    return *(M_BUFFER + M_RSCAT[I] + M_CSCAT[J]);
  }
  const T& operator() (const size_t I, const size_t J) const
  {
    return *(M_BUFFER + M_RSCAT[I] + M_CSCAT[J]); 
  }

  //size
  size_t size() const {return M_NELM;}
  size_t size(const size_t dim) const {return dim == 0 ? M_NROW : M_NCOL;}

  //Data operator
  T* data() {return M_BUFFER;}
  const T* data() const {return M_BUFFER;}

};//end of class

//------------------------------------------------------------------------
// m_set_default
//	sets the default data
//------------------------------------------------------------------------
template <typename T>
void scatter_matrix<T>::m_set_default()
{
  M_BUFFER = NULL;
  M_RSCAT.clear();
  M_CSCAT.clear();
  M_NELM = 0;
  M_NROW = 0;
  M_NCOL = 0;
}

//------------------------------------------------------------------------
// empty initialization
//------------------------------------------------------------------------
template <typename T>
scatter_matrix<T>::scatter_matrix()
{
  m_set_default();
}

//------------------------------------------------------------------------
// copy constructor 
//------------------------------------------------------------------------
template <typename T>
scatter_matrix<T>::scatter_matrix(const libj::tensor_matrix<T>& TMAT)
{
  //set the buffer
  M_BUFFER = const_cast<T*>(TMAT.data());

  //Set the scatter matrices
  m_set_scatter(TMAT);
}

//------------------------------------------------------------------------
// Copy assignment
//------------------------------------------------------------------------
template <typename T>
scatter_matrix<T>& scatter_matrix<T>::operator=(const libj::tensor_matrix<T>& TMAT)
{
  M_BUFFER = const_cast<T*>(TMAT.data()); 

  //set the scatter matrices
  m_set_scatter(TMAT);

  return *this;
}

//------------------------------------------------------------------------
// m_set_scatter
//	given some tensor matrix TMAT, sets the scatter row and col vectors
//	for this scatter matrix
//------------------------------------------------------------------------
template <typename T>
void scatter_matrix<T>::m_set_scatter(const libj::tensor_matrix<T>& TMAT)
{
  M_NROW = TMAT.size(0);
  M_NCOL = TMAT.size(1);
  M_NELM = M_NROW * M_NCOL;

  if (M_NELM <= 0) 
  {
    printf("ERROR libj::scatter_matrix::m_set_scatter \n");
    printf("There are <= 0 elements in input tensor matrix\n");
    exit(1);
  }

  //set the row and scatter matrices
  M_RSCAT.resize(M_NROW);
  M_CSCAT.resize(M_NCOL);
  
  //set the row scatter vector
  for (size_t I=0;I<M_NROW;I++)
  {
    M_RSCAT[I] = TMAT.offset(I,0) - TMAT.offset(0,0);
  }

  //set the col scatter vector
  for (size_t J=0;J<M_NCOL;J++)
  {
    M_CSCAT[J] = TMAT.offset(0,J) - TMAT.offset(0,0);
  }
  
}

}//end of namespace

#endif
