/*-----------------------------------------------------------------------------
 * cache.hpp
 *  JHT, May 7, 2022 : created
 *
 *  .hpp file for the cache struct, which provides and interface to 
 *  stacked byte arrays of size par with L1 and L2 cache, and additional
 *  functionality to determine how many elements a particular type can be
 *  stored in each level of cache
 *
 *  Note that each cache carries with it an extra LINE_BYTES, in order to 
 *  mitigate false sharing. Additionally, 
 *
 *  USAGE
 *  ------------------
 *  //Initialization
 *  libj::Cache cache;
 *
 *  //Retrieve a pointer to different cache levels
 *  double* x = cache.L1_pointer<double>(); 
 *  const float* x = cache.L2_pointer<float>(); 
 *
 *  //Determine the number of elements of a given type in cache
 *  num_double = cache.L1_elements<double>();
-----------------------------------------------------------------------------*/
#include <stdlib.h>
#include "libjdef.h"

namespace libj
{

struct Cache
{
  public:
  //data
  alignas(LIBJ_MAX_ALIGN) char L1_CACHE[LIBJ_L1_BYTES+LIBJ_LINE_BYTES];
  alignas(LIBJ_MAX_ALIGN) char L2_CACHE[LIBJ_L2_BYTES+LIBJ_LINE_BYTES];
  alignas(LIBJ_MAX_ALIGN) char CACHE_LINE[LIBJ_LINE_BYTES];

  //functions
  template<typename T>
  T* L1_pointer() {
    return (T*) L1_CACHE;
  }
  template<typename T>
  const T* L1_pointer() const {
    return (T*) L1_CACHE;
  }

  template<typename T>
  T* L2_pointer() {
    return (T*) L2_CACHE;
  }
  template<typename T>
  const T* L2_pointer() const {
    return (T*) L2_CACHE;
  }
  
  template<typename T>
  T* LINE_pointer() {
    return (T*) CACHE_LINE; 
  }
  template<typename T>
  const T* LINE_pointer() const {
    return (T*) CACHE_LINE; 
  }

  //functions to get numbers of elements in cache
  template<typename T>
  static constexpr size_t L2_elements() 
  {
      return LIBJ_L2_BYTES / sizeof(T);
  } 

  template<typename T>
  static constexpr size_t L1_elements() 
  {
      return LIBJ_L1_BYTES / sizeof(T);
  } 

  template<typename T>
  static constexpr size_t LINE_elements() 
  {
      return LIBJ_LINE_BYTES / sizeof(T);
  } 

}; //cache struct 

}//end namespace
