/*---------------------------------------------------------------------------------------
  index_bundle2.hpp
	JHT, April 27, 2022 : created

  class which contains information about index bundles

  This one has been optimized for performance

  Functionality
  ------------------
  bunde.offset(index);  //returns the offset of this element in the original tensor
---------------------------------------------------------------------------------------*/

#ifndef INDEX_BUNDLE2_HPP
#define INDEX_BUNDLE2_HPP

#include <array>
#include <vector>
#include <string>
#include <algorithm> 
#include <stdlib.h>
#include "tensor.hpp"

#define INDEX_UNROLL 4

namespace libj
{
//------------------------------------------------------------------------
// index struct for fast access
//------------------------------------------------------------------------
struct index
{
  size_t STRIDE;
  size_t LENGTH;
  size_t LDA;
};

//------------------------------------------------------------------------
// index bundle class
//------------------------------------------------------------------------
template <typename T, size_t NDIM>
struct index_bundle2
{
  size_t                   NELM;   //number of elements
  size_t                   START;  //starting index (for blocking)
  std::array<size_t,NDIM>  DIM;    //dimension list that maps between bundle and original
  std::array<index,NDIM>   IDX;    //index list

  //copy constructor
  index_bundle2(const index_bundle2& other)
  {
    NELM  = other.NELM;
    START = other.START;
    DIM   = other.DIM;
    IDX   = other.IDX;
  }

  //copy assignment
  index_bundle2& operator= (const index_bundle2& other)
  {
    NELM  = other.NELM;
    START = other.START;
    DIM   = other.DIM;
    IDX   = other.IDX;
    return *this;
  }

  size_t size() const {return NELM;}

  //given cumulative index I, return the value for sub-index idx
  size_t get_index(const size_t I, const size_t idx) const
  {
    return ((I+START)/IDX[idx].STRIDE)%IDX[idx].LENGTH;
  }

  //convert char to dimension number
  size_t c2dim(const char c) const
  {
    return (size_t) (std::tolower(c) - (int) 'a');
  }

  //make the bundle
  void make_bundle(const libj::tensor<T>& tens, const std::string& str)
  {
    const size_t dim = c2dim(str[0]);
    DIM[0] = dim;
    IDX[0].LENGTH = tens.size(dim);
    IDX[0].STRIDE = 1;
    IDX[0].LDA = tens.stride(dim);
    NELM = IDX[0].LENGTH;

    for (size_t idx=1;idx<NDIM;idx++)
    {
      const size_t dd = c2dim(str[idx]);
      DIM[idx] = dd;
      IDX[idx].LENGTH = tens.size(dd);
      IDX[idx].STRIDE = IDX[idx-1].STRIDE*IDX[idx-1].LENGTH;
      IDX[idx].LDA = tens.stride(dd);
      NELM *= IDX[idx].LENGTH; 
    }
  }

  //return a new index bundle as a block of the current index bundle
  //  starting at bundled index I and going to bundled index I+(MIN:MAX - I,LEN)
  index_bundle2 block(const size_t I, const size_t LEN) const
  {
    index_bundle2<T,NDIM> block;
    block.START = I; //here is the big trick
    block.NELM = std::min(LEN,NELM - I);
    block.DIM = DIM;
    block.IDX = IDX;
    return block;
  }

  //returns the offset of an index of this particular bundle in the 
  //  original tensor
  size_t offset(const size_t I) const
  {
    size_t off=0;
    for (size_t dim=0;dim<NDIM;dim++)
    {
      off += IDX[dim].LDA*get_index(I,dim); 
    }  
    return off;
  }

}; //end class

}//end namespace

#endif
