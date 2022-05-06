/*----------------------------------------------------------------------------
  block_scatter_matrix.hpp
	JHT, April 29, 2022 : created

  .hpp file for the block_scatter_matrix class, which is used to access a 
  tensor_matrix in an out-of-order fashion. This is the blocked 
  version of this   

  This is constructed from a tensor_matrix, which has already performed the
  matrixification proccess. 

  NOTE : we const_cast the buffer pointer from the references tensor_matrix,
         be super careful that you are calling const when you need to be...

  Useful functions
  ------------------------
  
  Blocking information:
  T.block_num(dim); 		//number of blocks in dimension "dim"
  T.block_size(dim);		//size of blocks in dimension "dim"
  T.block_id(dim,index);	//block which index falls into in dimension "dim"
  T.block_stride(dim,block);	//stride of block "block" in dimension "dim"
  T.next_block_index(dim,index);//returns the starting index of the next block

----------------------------------------------------------------------------*/
#ifndef BLOCK_SCATTER_MATRIX_HPP
#define BLOCK_SCATTER_MATRIX_HPP

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
// block_scatter_matrix class
//------------------------------------------------------------------------
template <typename T>
class block_scatter_matrix
{
  private:
  //data
  T*                     M_BUFFER;	//base pointer
  std::vector<size_t>    M_RSCAT;       //row scatter vector
  std::vector<size_t>    M_CSCAT;       //col scatter vector
  std::vector<size_t>    M_RBS;		//row block scatter vector
  std::vector<size_t>    M_CBS;         //column block scatter vector
  size_t                 M_NELM;        //number of total elements
  size_t                 M_NROW;        //number of rows 
  size_t                 M_NCOL;        //number of cols 
  size_t                 M_NRB;         //number of row blocks
  size_t                 M_NCB;         //number of col blocks
  size_t                 M_RBL;		//size of row blocks
  size_t                 M_CBL;		//size of col blocks
  bool                   M_SEQ;         //bool tracking if is sequential

  //internal functions
  void m_set_default();
  void m_set_scatter(const libj::tensor_matrix<T>& TMAT, const size_t RBL, const size_t CBL);
  void m_set_block(const size_t RBL, const size_t CBL);
  
  public:

  //initialization
  block_scatter_matrix();

  //copy constructor
  block_scatter_matrix(const libj::tensor_matrix<T>& TMAT, const size_t RBL, 
                       const size_t CBL);

  //assignment constructor
  void assign(const libj::tensor_matrix<T>& TMAT, 
              const size_t RBL, const size_t CBL);

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

  //block information
  size_t block_num(const size_t dim) const {return dim == 0 ? M_NRB : M_NCB;}
  size_t block_size(const size_t dim) const {return dim == 0 ? M_RBL : M_CBL;}
  size_t block_size(const size_t dim, const size_t block) const 
  {
    return dim == 0 ? std::min(M_RBL,M_NROW - (M_RBL*block)) 
                    : std::min(M_CBL,M_NCOL - (M_CBL*block));}
  size_t block_stride(const size_t dim, const size_t block) const
  {
    return dim == 0 ? M_RBS[block] : M_CBS[block];
  }
  size_t block_id(const size_t dim, const size_t index)
  {
    return dim == 0 ? std::min(index,M_NROW) / M_RBL
                    : std::min(index,M_NCOL) / M_CBL; 
  }
  size_t next_block_index(const size_t dim, const size_t index)
  {
    return dim == 0 ? M_RBL*(block_id(0,index)+1)
                    : M_CBL*(block_id(1,index)+1);
  }

  //Data operator
  T* data() {return M_BUFFER;}
  const T* data() const {return M_BUFFER;}

};//end of class

//------------------------------------------------------------------------
// m_set_default
//	sets the default data
//------------------------------------------------------------------------
template <typename T>
void block_scatter_matrix<T>::m_set_default()
{
  M_BUFFER = NULL;
  M_RSCAT.clear();
  M_CSCAT.clear();
  M_RBS.clear();
  M_CBS.clear();
  M_NELM = 0;
  M_NROW = 0;
  M_NCOL = 0;
  M_NRB  = 0;
  M_NCB  = 0;
  M_RBL  = 0;
  M_CBL  = 0;
}

//------------------------------------------------------------------------
// empty initialization
//------------------------------------------------------------------------
template <typename T>
block_scatter_matrix<T>::block_scatter_matrix()
{
  m_set_default();
}

//------------------------------------------------------------------------
// copy constructor 
//------------------------------------------------------------------------
template <typename T>
block_scatter_matrix<T>::block_scatter_matrix(const libj::tensor_matrix<T>& TMAT,
                                              const size_t RBL, const size_t CBL)
{
  //set the buffer
  M_BUFFER = const_cast<T*>(TMAT.data());

  //Set the scatter matrices
  m_set_scatter(TMAT,RBL,CBL);
}

//------------------------------------------------------------------------
// Copy assignment
//------------------------------------------------------------------------
template <typename T>
void block_scatter_matrix<T>::assign(const libj::tensor_matrix<T>& TMAT,
                                     const size_t RBL, const size_t CBL)
{
  M_BUFFER = const_cast<T*>(TMAT.data()); 

  //set the scatter matrices
  m_set_scatter(TMAT,RBL,CBL);

}

//------------------------------------------------------------------------
// m_set_scatter
//	given some tensor matrix TMAT, sets the scatter row and col vectors
//	for this scatter matrix
//------------------------------------------------------------------------
template <typename T>
void block_scatter_matrix<T>::m_set_scatter(const libj::tensor_matrix<T>& TMAT, 
                                            const size_t RBL, const size_t CBL)
{
  M_NROW = TMAT.size(0);
  M_NCOL = TMAT.size(1);
  M_NELM = M_NROW * M_NCOL;

  if (M_NELM <= 0) 
  {
    printf("ERROR libj::block_scatter_matrix::m_set_scatter \n");
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

  //set the block paramters
  m_set_block(RBL,CBL);
  
}

//------------------------------------------------------------------------
// m_set_block
//	sets the blocking sizes based on the scatter vectors
//------------------------------------------------------------------------
template <typename T>
void block_scatter_matrix<T>::m_set_block(const size_t RBL,const size_t CBL)
{
  //set the block lengths
  M_RBL = RBL;
  M_CBL = CBL;

  //set block number
  M_NRB = (M_NROW + M_RBL - 1) / M_RBL;
  M_NCB = (M_NCOL + M_CBL - 1) / M_CBL; 

  //resize the vectors if needed
  M_RBS.resize(M_NRB);
  M_CBS.resize(M_NCB);

  //Construct row blocks
  for (size_t block=0; block < M_NRB; block++)
  {
    //determine row strides
    const size_t end = std::min(M_NROW,(block+1)*M_RBL);
    const size_t start = block*M_RBL;
    size_t stride;
    if (end - start > 1)
    {
      stride = M_RSCAT[start+1] - M_RSCAT[start];
      for (size_t I = start + 2; I < end ; I++)
      {
        if (M_RSCAT[I] - M_RSCAT[I-1] != stride) 
        {
          stride = 0; 
          break;
        }
      }

    //only one element 
    } else {
      stride = 1; 
    }

    M_RBS[block] = stride;
  }

  //Construct col blocks
  for (size_t block=0; block < M_NCB; block++)
  {
    //determine row strides
    size_t stride;
    const size_t end = std::min(M_NCOL,(block+1)*M_CBL);
    const size_t start = block*M_CBL;
    if (end - start > 1)
    {
      stride = M_CSCAT[start+1] - M_CSCAT[start];
      for (size_t J = start + 2; J < end ; J++)
      {
        if (M_CSCAT[J] - M_CSCAT[J-1] != stride) 
        {
          stride = 0; 
          break;
        }
      }

    //only one element 
    } else {
      stride = 1; 
    }

    M_CBS[block] = stride;
  }

}


}//end of namespace

#endif
