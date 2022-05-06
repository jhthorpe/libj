/*---------------------------------------------------------------------------------------
  index_bundle.hpp
	JHT, April 27, 2022 : created

  class which contains information about index bundles
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
  size_t              NDIM;   //number of dimensions
  size_t              NELM;   //number of elements
  size_t              START;  //starting index (for blocking)
  std::vector<size_t> DIM;    //dimension list
  std::vector<size_t> STRIDE; //stride list
  std::vector<size_t> LENGTH; //length list

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
  void make_bundle(libj::tensor<T>& tens, const std::string& str)
  {
    NDIM = str.length();

    DIM.resize(NDIM);
    STRIDE.resize(NDIM);
    LENGTH.resize(NDIM);
   
    NELM = 1;
    for (size_t idx=0;idx<NDIM;idx++)
    {
      const size_t dim = c2dim(str[idx]);
      DIM[idx] = dim;
      LENGTH[idx] = tens.size(dim);
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
    return block;
  }

}; //end class

}//end namespace

#endif
