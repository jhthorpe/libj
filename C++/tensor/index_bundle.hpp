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
// index bundle class
//------------------------------------------------------------------------
struct index_bundle
{
  size_t              NDIM;   //number of dimensions in bundle
  size_t              NELM;   //number of elements
  size_t              START;  //starting index (for blocking)
  std::vector<size_t> DIM;    //dimension list that maps between bundle and original
  std::vector<size_t> STRIDE; //stride list of each dimension in THIS bundle
  std::vector<size_t> LENGTH; //length list of each dimension in original tensor
  std::vector<size_t> LDA;    //stride list of each dimension in orignal tensor

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
    STRIDE = other.STRIDE;
    LENGTH = other.LENGTH;
    LDA = other.LDA;
  }

  //copy assignment
  index_bundle& operator= (const index_bundle& other)
  {
    NDIM = other.NDIM;
    NELM = other.NELM;
    START = other.START;
    DIM.resize(NDIM);
    STRIDE.resize(NDIM);
    LENGTH.resize(NDIM);
    DIM = other.DIM;
    STRIDE = other.STRIDE;
    LENGTH = other.LENGTH;
    LDA = other.LDA;
    return *this;
  }

  //clear the data in the bundle
  void clear()
  {
    NDIM = 0;
    NELM = 0;
    START = 0;
    DIM.clear();
    STRIDE.clear();
    LENGTH.clear();
    LDA.clear();
  }

  size_t size() const {return NELM;}

  //given cumulative index I, return the value for sub-index idx
  size_t get_index(const size_t I, const size_t idx) const
  {
    return ((I+START)/STRIDE[idx])%LENGTH[idx];
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
    if (STRIDE.size() < NDIM) {STRIDE.resize(NDIM);}
    if (LENGTH.size() < NDIM) {LENGTH.resize(NDIM);}
    if (LDA.size() < NDIM) {LDA.resize(NDIM);}
   
    NELM = 1;
    for (size_t idx=0;idx<NDIM;idx++)
    {
      const size_t dim = c2dim(str[idx]);
      DIM[idx] = dim;
      LENGTH[idx] = tens.size(dim);
      LDA[idx] = tens.stride(dim);
      idx > 0 ? STRIDE[idx] = STRIDE[idx-1]*LENGTH[idx-1] : STRIDE[0] = 1;
      NELM *= LENGTH[idx]; 
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
    block.STRIDE = STRIDE;
    block.LENGTH = LENGTH;
    block.LDA = LDA;
    return block;
  }

  //returns the offset of an index of this particular bundle in the 
  //  original tensor
  size_t offset(const size_t I) const
  {
    size_t off=0;
    for (size_t dim=0;dim<NDIM;dim++)
    {
      off += LDA[dim]*get_index(I,dim); 
    }  
    return off;
  }

}; //end class

}//end namespace

#endif
