/*---------------------------------------------------------------------------------------
  index_bundle.hpp
	JHT, April 27, 2022 : created

  class which contains information about index bundles

  Functionality
  ------------------
  bunde.offset(index);  //returns the offset of this element in the original tensor
---------------------------------------------------------------------------------------*/

#ifndef INDEX_BUNDLE_HPP
#define INDEX_BUNDLE_HPP

#include <vector>
#include <string>
#include <algorithm> 
#include <stdlib.h>

namespace libj
{
//------------------------------------------------------------------------
// index struct 
//------------------------------------------------------------------------
struct index
{
  size_t LENGTH;
  size_t STRIDE;
  size_t LDA;
};
//------------------------------------------------------------------------
// index bundle class
//------------------------------------------------------------------------
struct index_bundle
{
  size_t              NDIM;   //number of dimensions in bundle
  size_t              NELM;   //number of elements
  size_t              START;  //starting index (for blocking)
  std::vector<size_t> DIM;    //dimension list that maps between bundle and original
  std::vector<index>  IDX;    //vector of structs to help with locality  

  //initialize the bundle to nothing
  index_bundle()
  {
    clear();
  }

  //copy constructor
  index_bundle(const index_bundle& other)
  {
    NDIM = other.NDIM;
    NELM = other.NELM;
    START = other.START;
    DIM = other.DIM;
    IDX = other.IDX;
  }

  //copy assignment
  index_bundle& operator= (const index_bundle& other)
  {
    NDIM = other.NDIM;
    NELM = other.NELM;
    START = other.START;
    if (DIM.size() < NDIM) {DIM.resize(NDIM);}
    if (IDX.size() < NDIM) {DIM.resize(NDIM);}
    DIM = other.DIM;
    IDX = other.IDX;
    return *this;
  }

  //clear the data in the bundle
  void clear()
  {
    NDIM = 0;
    NELM = 0;
    START = 0;
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
  template<typename T>
  void make_bundle(const libj::tensor<T>& tens, const std::string& str)
  {
    NDIM = str.length();


    if (DIM.size() < NDIM) {DIM.resize(NDIM);}
    if (IDX.size() < NDIM) {IDX.resize(NDIM);}

    NELM = 1; //key for "empty" bundles
    if (NDIM > 0)
    {
      const size_t d = c2dim(str[0]);
      DIM[0] = d;
      IDX[0].LENGTH = tens.size(d); 
      IDX[0].STRIDE = 1;
      IDX[0].LDA = tens.stride(d);
      NELM *= IDX[0].LENGTH;

      for (size_t idx=1;idx<NDIM;idx++)
      {
        const size_t dim = c2dim(str[idx]);
        DIM[idx] = dim;
        IDX[idx].LENGTH = tens.size(dim); 
        IDX[idx].STRIDE = IDX[idx-1].STRIDE * IDX[idx-1].LENGTH;
        IDX[idx].LDA = tens.stride(dim);
        NELM *= IDX[idx].LENGTH;
      }
    }
  }

  //return a new index bundle as a block of the current index bundle
  //  starting at bundled index I and going to bundled index I+(MIN:MAX - I,LEN)
  index_bundle block(const size_t I, const size_t LEN) const
  {
    index_bundle block;
    block.NDIM = NDIM;
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

  //calculates offsets for a block of numbers
  void offset_block(const size_t N, const size_t* I, size_t* off)
  {
    for (size_t i=0;i<N;i++)
    {
      off[i] = offset(I[i]);
    }
  }

}; //end class

}//end namespace

#endif
