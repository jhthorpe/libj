/*----------------------------------------------------------------------------
  tensor_matrix2.hpp
	JHT, April 25, 2022 : created
	JHT, April 25, 2022 : changed to array template

  .hpp file for the tensor_matrix2 class, which is used to "matrixicize" 
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
  libj::tensor_matrix2<double> A;
  A.assign(tensor,"abc","d"); 			
  libj::tensor_matrix2<int> B(tensor,"","");	//yields a col-vector rep. of tensor
 
  libj::tensor

  Matrix-style access
  ---------------------
  A(I,J); 

  Creating a subblock
  ---------------------
  libj::tensor_matrix2<double> T2 = T1.block(ROW,COL,NUM_ROWS,NUM_COLS); 

  Useful functions
  ---------------------
  A.data();		//returns pointer to data buffer
  A.offset(I,J); 	//returns of offset from data buffer 
				for bundled indicies

----------------------------------------------------------------------------*/
#ifndef TENSOR_MATRIX2_HPP
#define TENSOR_MATRIX2_HPP

#include "tensor.hpp"
#include "index_bundle2.hpp"
#include "alignment.hpp"
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>

namespace libj
{

//------------------------------------------------------------------------
// tensor_matrix2 class
//------------------------------------------------------------------------
template <typename T, size_t NLHS, size_t NRHS>
class tensor_matrix2
{
  private:
  //data
  T*                          M_BUFFER;     //pointer to base data, offset in case of block
  libj::tensor<T>             M_TENSOR;     //pointer to tensor this represents
  libj::index_bundle2<NLHS>   M_LHS;	     //left hand side index bundles
  libj::index_bundle2<NRHS>   M_RHS;        //right hand size index bundles

  //Internal functions
  void m_set_default(); //set the default values
  void m_set_dimensions(const std::string& lhs, const std::string& rhs);
  bool m_good_bundles();

  public:
  //Constructors
  tensor_matrix2();

  //Copy constructor from tensor
  tensor_matrix2(const libj::tensor<T>& tens, 
                 const std::string& lhs, const std::string& rhs);

  //Assignment
  void assign(libj::tensor<T>& tens, 
              const std::string& lhs, const std::string& rhs);
  void assign(const libj::tensor<T>& tens, 
              const std::string& lhs, const std::string& rhs);

  //getters
  size_t size() const {return M_LHS.NELM * M_RHS.NELM;} 
  size_t size(const size_t dim) const 
  {
    return dim == 0? M_LHS.NELM : M_RHS.NELM;
  }
  size_t bundle_size(const size_t side)
  {
    return side == 0? M_LHS.NDIM : M_RHS.NDIM;
  }

  constexpr size_t dim() const {return NLHS*NRHS;}
  constexpr size_t dim(const size_t side) const {
    return side == 0 ? NLHS : NRHS;
  }

  //access functions
  T& operator() (const size_t I, const size_t J);
  const T& operator() (const size_t I, const size_t J) const;

  //offset functions
  size_t offset(const size_t I, const size_t J) const;
  void offset_col(const size_t I, const size_t J,
                  const size_t NI, const size_t rel,
                  size_t* off) const;
  void offset_row(const size_t I, const size_t J,
                  const size_t NJ, const size_t rel,
                  size_t* off) const;

  //data function
  T* data() {return M_BUFFER;}
  const T* data() const {return M_BUFFER;} 

  //block function
  tensor_matrix2<T,NLHS,NRHS> block(const size_t ROW, const size_t COL, 
                                    const size_t NROW, const size_t NCOL);
  const tensor_matrix2<T,NLHS,NRHS> block(const size_t ROW, 
                                          const size_t COL, 
                                          const size_t NROW, 
                                          const size_t NCOL) const;

};//end of class

//-----------------------------------------------------------------------------------------
// set default values
//-----------------------------------------------------------------------------------------
template<typename T, size_t NLHS, size_t NRHS>
void tensor_matrix2<T,NLHS,NRHS>::m_set_default()
{
  //don't set the tensor yet
//  M_BUFFER = NULL;
}

//-----------------------------------------------------------------------------------------
// empty constructor 
//-----------------------------------------------------------------------------------------
/*
template <typename T, size_t NLHS, size_t NRHS>
tensor_matrix2<T,NLHS,NRHS>::tensor_matrix2()
{
  //m_set_default();
}
*/

//-----------------------------------------------------------------------------------------
// copy constructor 
//-----------------------------------------------------------------------------------------
template <typename T,size_t NLHS, size_t NRHS>
tensor_matrix2<T,NLHS,NRHS>::tensor_matrix2(const libj::tensor<T>& tens, 
                                            const std::string& lhs,
                                            const std::string& rhs)
{
//  m_set_default();
  assign(tens,lhs,rhs);
}

//-----------------------------------------------------------------------------------------
// m_set_dimensions
//	sets the bundles and dimensions of the tensor_matrix2
//-----------------------------------------------------------------------------------------
template<typename T, size_t NLHS, size_t NRHS>
void tensor_matrix2<T,NLHS,NRHS>::m_set_dimensions(const std::string& lhs, 
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
//    M_LHS.make_bundle<T>(M_TENSOR,new_lhs);
//    M_RHS.make_bundle<T>(M_TENSOR,"");
    M_LHS.make_bundle(M_TENSOR,new_lhs);
    M_RHS.make_bundle(M_TENSOR,"");
  } else {
    //go through each and make the bundles 
//    M_LHS.make_bundle<T>(M_TENSOR,lhs);
//    M_RHS.make_bundle<T>(M_TENSOR,rhs);
    M_LHS.make_bundle(M_TENSOR,lhs);
    M_RHS.make_bundle(M_TENSOR,rhs);
  }

  //check if the bundles were good
  if (!m_good_bundles())
  { 
    printf("ERROR libj::tensor_matrix2::m_set_dimensions \n");
    printf("The input bundles are bad \n");
    printf("LHS = %s \n",lhs.c_str());
    printf("RHS = %s \n",rhs.c_str());
    exit(1);
  }

}

//-----------------------------------------------------------------------------------------
// m_check_bundles
//-----------------------------------------------------------------------------------------
template <typename T, size_t NLHS, size_t NRHS>
bool tensor_matrix2<T,NLHS,NRHS>::m_good_bundles()
{
  //check that the number of dimensions in each sums to tensor dimensions
  if (M_LHS.dim() + M_RHS.dim() != M_TENSOR.dim()) {return false;}

  //check that each dimension only appears once
  constexpr size_t TOT = NLHS+NRHS; 
  std::array<size_t,TOT> COUNT;
  for (size_t dim = 0; dim < TOT ; dim++) {COUNT[dim] = 0;} 
  for (size_t idx = 0; idx < M_LHS.dim(); idx++) {COUNT.at(M_LHS.DIM[idx])++;}
  for (size_t idx = 0; idx < M_RHS.dim(); idx++) {COUNT.at(M_RHS.DIM[idx])++;}
  for (size_t dim = 0; dim < TOT; dim++) {if (COUNT[dim] != 1) {return false;}}
  return true;
}


//-----------------------------------------------------------------------------------------
// Assigment 
//-----------------------------------------------------------------------------------------
template<typename T, size_t NLHS, size_t NRHS>
void tensor_matrix2<T,NLHS,NRHS>::assign(libj::tensor<T>& tens, 
                                         const std::string& lhs, 
                                         const std::string& rhs)
{
  //reset default values (incase the tensor_matrix2 is already set)
  m_set_default();

  //assign the pointer
  M_TENSOR = tens;
  M_BUFFER = M_TENSOR.data(); 

  //set the dimensions
  m_set_dimensions(lhs,rhs);
}

template<typename T, size_t NLHS, size_t NRHS>
void tensor_matrix2<T,NLHS,NRHS>::assign(const libj::tensor<T>& tens, 
                                         const std::string& lhs, 
                                         const std::string& rhs)
{
  //reset default values (incase the tensor_matrix2 is already set)
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
template <typename T, size_t NLHS, size_t NRHS>
T& tensor_matrix2<T,NLHS,NRHS>::operator() (const size_t I, const size_t J)
{
  //dummy array 
  std::vector<size_t> DUMMY(M_LHS.NDIM + M_RHS.NDIM); 

  //LHS
  for (size_t idx = 0; idx < M_LHS.NDIM; idx++)
  {
    const size_t dim = M_LHS.DIM[idx];
    DUMMY[dim] = M_LHS.get_index(I,idx);
  }

  //RHS
  for (size_t idx = 0; idx < M_RHS.NDIM; idx++)
  {
    const size_t dim = M_RHS.DIM[idx];
    DUMMY[dim] = M_RHS.get_index(J,idx);
  }
 
  return M_TENSOR(DUMMY);
}

template <typename T, size_t NLHS, size_t NRHS>
const T& tensor_matrix2<T,NLHS,NRHS>::operator() (const size_t I, 
                                                  const size_t J) const
{
  std::vector<size_t> vec(M_LHS.dim() + M_RHS.dim());
  //LHS
  for (size_t idx = 0; idx < M_LHS.dim(); idx++)
  {
    const size_t dim = M_LHS.DIM[idx];
    vec[dim] = M_LHS.get_index(I,idx);
  }

  //RHS
  for (size_t idx = 0; idx < M_RHS.dim(); idx++)
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
template <typename T, size_t NLHS, size_t NRHS>
size_t tensor_matrix2<T,NLHS,NRHS>::offset(const size_t I, 
                                           const size_t J) const
{
  return M_LHS.offset(I) + M_RHS.offset(J);
}

//-----------------------------------------------------------------------------------------
// offset_col
// calculates a block of offsets starting with I (LHS)and going to I+NI-1, with a fixed
//   J(RHS)
//   
//   I and J    : the starting rows and cols for the offset
//   NI         : the number of rows to calculate
//   rel        : an offset to calculate this relative to
//   off*       : pointer to offset data
//-----------------------------------------------------------------------------------------
template <typename T, size_t NLHS, size_t NRHS>
inline void tensor_matrix2<T,NLHS,NRHS>::offset_col(const size_t I, 
                                                    const size_t J,
                                                    const size_t NI, 
                                                    const size_t rel,
                                                    size_t* off) const
{
  const size_t JOFF = M_RHS.offset(J); 
  size_t tmp = M_LHS.offset(I+0);
  for (size_t i=1;i<NI;i++)
  {
    tmp += JOFF;
    off[i-1] = tmp - rel;
    tmp = M_LHS.offset(I+i);
  } 
  tmp += JOFF;
  off[NI-1] = tmp - rel;
}

//-----------------------------------------------------------------------------------------
// offset_row
// calculates a block of offsets starting with J (RHS)and going to J+NJ-1, with a fixed
//   I(RHS)
//-----------------------------------------------------------------------------------------
template <typename T, size_t NLHS, size_t NRHS>
inline void tensor_matrix2<T,NLHS,NRHS>::offset_row(const size_t I, 
                                                    const size_t J,
                                                    const size_t NJ, 
                                                    const size_t rel,
                                                    size_t* off) const
{
  const size_t IOFF = M_LHS.offset(I); 
  size_t tmp = M_RHS.offset(J+0);
  for (size_t j=1;j<NJ;j++)
  {
    tmp += IOFF;
    off[j-1] = tmp - rel;
    tmp = M_RHS.offset(J+j);
  } 
  tmp += IOFF;
  off[NJ-1] = tmp - rel;
}

//-----------------------------------------------------------------------------------------
// offset_block
// calculates a block of offsets starting with I (LHS),J(RHS) and going to I+NI-1 and J+NJ-1
//  
// this is stored as a column major matrix, with J as the cols
//           J    J+1  ...   J+NJ-1
//  I        
//  I+1
//  ...
//  I+NI-1	
//  
//  This routine is based on the following trick. The ROW contributions are the same for
//  each COL. Therefor they only need to be computed once, and copied from one col to 
//  another. Similarly, each set of rows only needs to be added by a single 
//  value of J offset. 
//  
//-----------------------------------------------------------------------------------------
/*
template <typename T>
void tensor_matrix2<T>::offset_block(const size_t I, const size_t J,
                                    const size_t NI, const size_t NJ,
                                    size_t* off) const
{
  printf("ERROR tensor_matrix2::offset_block \n") ;
  printf("JHT really needs to code this \n");
  exit(1);
}
*/

//-----------------------------------------------------------------------------------------
// block 
//	Note that this block really comes down to altering the index bundles, as they track
//	the actual block position within the original tensor
//-----------------------------------------------------------------------------------------
template <typename T, size_t NLHS, size_t NRHS>
tensor_matrix2<T,NLHS,NRHS> tensor_matrix2<T,NLHS,NRHS>::block(const size_t ROW, const size_t COL,const size_t NROW, const size_t NCOL)
{
  tensor_matrix2<T,NLHS,NRHS> TENS;
  TENS.M_TENSOR = M_TENSOR; //same source tensor
  TENS.M_LHS = M_LHS.block(ROW,NROW); //LHS bundles change
  TENS.M_RHS = M_RHS.block(COL,NCOL); //RHS bundles change
  TENS.M_BUFFER = &(*this)(ROW,COL); 
  return TENS; 
}

template <typename T, size_t NLHS, size_t NRHS>
const tensor_matrix2<T,NLHS,NRHS> tensor_matrix2<T,NLHS,NRHS>::block(const size_t ROW, const size_t COL, const size_t NROW, const size_t NCOL) const
{
  tensor_matrix2<T,NLHS,NRHS> TENS;
  TENS.M_TENSOR = M_TENSOR; //same source tensor
  TENS.M_LHS = M_LHS.block(ROW,NROW); //LHS bundles change
  TENS.M_RHS = M_RHS.block(COL,NCOL); //RHS bundles change
  TENS.M_BUFFER = &(*this)(ROW,COL); 
  return TENS; 
}


}//end of namespace

#endif
