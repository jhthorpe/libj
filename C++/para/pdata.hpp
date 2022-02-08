/*------------------------------------------------------------------------
 * pdata.hpp
 *  JHT, Febuary 7, 2022 : created
 *
 *  .hpp file for Pdata, which manages a (possibly parallel) set of data
 *
 *  Things we need:
 *  Data lists 
 *    - list number and sublist id as identifiers 
 *    - size, file id, file position
 *
 *  Files
 *    - file name
 *    - file pointer
 *    - isopen bool
 *    - open/close file
 *    - addfile
 *
 *  Functions
 *    - getlist     : handles read and send 
 *    - putlist     : handles send and write
 *    - addlist     : add a list to track
 *
 *  Index 
 *    - tracks which lists are currently incore
 *    - reads 
------------------------------------------------------------------------*/
#ifndef PDATA_HPP
#define PDATA_HPP

#include "libjdef.hpp"

class Pdata
{
  private:

  public:

  
};

#endif
