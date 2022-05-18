/*----------------------------------------------------------------------------
  block_scatter_matrix.hpp
	JHT, April 29, 2022 : created
    JHt, May 17, 2022   : changed to std::array

  .hpp file for the block_scatter_matrix class, which is used to access a 
  tensor_matrix in an out-of-order fashion.

  Note that the number of rows/cols and block sizes must be known at runtime!!

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
#ifndef BLOCK_SCATTER_MATRIX2_HPP
#define BLOCK_SCATTER_MATRIX2_HPP

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
template <typename T, size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
class block_scatter_matrix
{
  private:
  //constexpr data
  static constexpr size_t M_NRB = (NROW + RBL - 1)/RBL;  //number of row blocks
  static constexpr size_t M_NCB = (NCOL + CBL - 1)/CBL;  //number of col blocks
  static constexpr size_t M_NELM = NROW * NCOL; //Number of total elements

  //data
  T*                         M_BUFFER;	//base pointer
  std::array<size_t,NROW>    M_RSCAT;   //row scatter vector
  std::array<size_t,NCOL>    M_CSCAT;   //col scatter vector
  std::array<size_t,M_NRB>   M_RBS;		//row block scatter vector
  std::array<size_t,M_NCB>   M_CBS;     //column block scatter vector
  bool                       M_SEQ;     //bool tracking if is sequential

  //internal functions
  void m_set_default();
  void m_set_block();
  
  template<size_t NLHS, size_t NRHS>
  void m_set_scatter(const libj::tensor_matrix<T,NLHS,NRHS>& TMAT);

  //test
  template<size_t NLHS, size_t NRHS>
  void m_rscat(const libj::tensor_matrix<T,NLHS,NRHS>& TMAT);
  template<size_t NLHS, size_t NRHS>
  void m_cscat(const libj::tensor_matrix<T,NLHS,NRHS>& TMAT);
  
  public:

  //initialization
  block_scatter_matrix();

  //copy constructor
  template<size_t NLHS, size_t NRHS>
  block_scatter_matrix(const libj::tensor_matrix<T,NLHS,NRHS>& TMAT); 

  //assignment constructor
  template<size_t NLHS, size_t NRHS>
  void assign(const libj::tensor_matrix<T,NLHS,NRHS>& TMAT);

  template<size_t NLHS, size_t NRHS>
  void assign_to_block(const libj::tensor_matrix<T,NLHS,NRHS>& TMAT,
                       const size_t ROW, const size_t COL);

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
  constexpr size_t size() const {return M_NELM;}
  constexpr size_t size(const size_t dim) const {return dim == 0 ? NROW : NCOL;}

  //block information
  constexpr size_t block_num(const size_t dim) const {return dim == 0 ? M_NRB : M_NCB;}
  constexpr size_t block_size(const size_t dim) const {return dim == 0 ? RBL : CBL;}
  size_t block_size(const size_t dim, const size_t block) const 
  {
    return dim == 0 ? std::min(RBL,NROW - (RBL*block)) 
                    : std::min(CBL,NCOL - (CBL*block));}
  size_t block_stride(const size_t dim, const size_t block) const
  {
    return dim == 0 ? M_RBS[block] : M_CBS[block];
  }
  size_t block_id(const size_t dim, const size_t index)
  {
    return dim == 0 ? std::min(index,NROW) / RBL
                    : std::min(index,NCOL) / CBL; 
  }
  size_t next_block_index(const size_t dim, const size_t index)
  {
    return dim == 0 ? RBL*(block_id(0,index)+1)
                    : CBL*(block_id(1,index)+1);
  }

  //Data operator
  T* data() {return M_BUFFER;}
  const T* data() const {return M_BUFFER;}

};//end of class

//------------------------------------------------------------------------
// m_set_default
//	sets the default data
//------------------------------------------------------------------------
template <typename T, size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
void block_scatter_matrix<T,NROW,NCOL,RBL,CBL>::m_set_default()
{
  M_BUFFER = NULL;
  std::fill_n(M_RSCAT,M_RSCAT.size(),0);
  std::fill_n(M_CSCAT,M_CSCAT.size(),0);
  std::fill_n(M_RBS,M_RBS.size(),0);
  std::fill_n(M_CBS,M_CBS.size(),0);
  M_NELM = 0;
  NROW = 0;
  NCOL = 0;
  M_NRB  = 0;
  M_NCB  = 0;
  RBL  = 0;
  CBL  = 0;
}

//------------------------------------------------------------------------
// empty initialization
//------------------------------------------------------------------------
template <typename T, size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
block_scatter_matrix<T,NROW,NCOL,RBL,CBL>::block_scatter_matrix()
{
}

//------------------------------------------------------------------------
// copy constructor 
//------------------------------------------------------------------------
template <typename T, size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
template <size_t NLHS, size_t NRHS>
block_scatter_matrix<T,NROW,NCOL,RBL,CBL>::block_scatter_matrix(
                    const libj::tensor_matrix<T,NLHS,NRHS>& TMAT)
{
  M_BUFFER = const_cast<T*>(TMAT.data()); 

  //set the scatter matrices
  m_set_scatter(TMAT);
}

//------------------------------------------------------------------------
// m_set_scatter
//	given some tensor matrix TMAT, sets the scatter row and col vectors
//	for this scatter matrix
//------------------------------------------------------------------------
template <typename T, size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
template <size_t NLHS, size_t NRHS>
void block_scatter_matrix<T,NROW,NCOL,RBL,CBL>::m_set_scatter(
                            const libj::tensor_matrix<T,NLHS,NRHS>& TMAT) 
{
  
  //set the row scatter vector
  const size_t off00 = TMAT.offset(0,0);
  for (size_t I=0;I<NROW;I++)
  {
    M_RSCAT[I] = TMAT.offset(I,0) - off00;
  }

  //set the col scatter vector
  for (size_t J=0;J<NCOL;J++)
  {
    M_CSCAT[J] = TMAT.offset(0,J) - off00;
  }

  //set the block paramters
  m_set_block();

}

//------------------------------------------------------------------------
// m_set_block
//	sets the blocking sizes based on the scatter vectors
//------------------------------------------------------------------------
template <typename T, size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
inline void block_scatter_matrix<T,NROW,NCOL,RBL,CBL>::m_set_block()
{
  //Construct row blocks
  M_RBS[0] = 1; //for empty bundle
  for (size_t block=0; block < M_NRB; block++)
  {
    //determine row strides
    const size_t end = std::min(NROW,(block+1)*RBL);
    const size_t start = block*RBL;
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
  M_CBS[0] = 1; //for empty bundle
  for (size_t block=0; block < M_NCB; block++)
  {
    //determine col strides
    size_t stride;
    const size_t end = std::min(NCOL,(block+1)*CBL);
    const size_t start = block*CBL;
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

//------------------------------------------------------------------------
// assign_to_block
//   assigns the block scatter matrix to a block of a tensor
//   
//   this is intended to be a high-speed interface to minimize overhead 
//------------------------------------------------------------------------
template <typename T,size_t NROW, size_t NCOL, size_t RBL, size_t CBL>
template <size_t NLHS, size_t NRHS>
inline void block_scatter_matrix<T, NROW, NCOL, RBL, CBL>::assign_to_block(
                              const libj::tensor_matrix<T,NLHS,NRHS>& TMAT, 
                                         const size_t ROW, const size_t COL)
{
  //assign the buffer
  M_BUFFER = const_cast<T*>(&TMAT(ROW,COL)); 

  //now, set the scatter vectors
  const size_t off00 = TMAT.offset(ROW,COL);
  M_RSCAT[0] = 0;
  TMAT.offset_col(ROW,COL,NROW,off00,M_RSCAT.data());
  M_CSCAT[0] = 0;
  TMAT.offset_row(ROW,COL,NCOL,off00,M_CSCAT.data());

  //set the block elements
  m_set_block();
}

}//end of namespace

#endif
