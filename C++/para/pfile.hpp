/*------------------------------------------------------------------------
 * pfile.hpp
 *  JHT, Febuary 8, 2022: created
 *
 *  .hpp file for Pfile, which handles a (possibly parallel) filesystem
 *
------------------------------------------------------------------------*/
#ifndef PFILE_HPP
#define PFILE_HPP

#include <string>
#include <vector>
#include <stdio.h>
#include "libjdef.h"

#define PFILE_RES 50

class Pfile
{
  private: 
  //Data
  std::vector<bool>        m_isopen;  //bools for tracking if file is open
  std::vector<bool>        m_isbin;   //bools if file is binary 
  std::vector<FILE*>       m_fptr;    //file pointers
  std::vector<long>        m_fpos;    //current file positions (bytes)
  std::vector<std::string> m_fname;   //file names
  std::vector<std::string> m_fstat;   //file status
  int                      m_nfiles;  //number of files

  public:
  //Initializer
  Pfile();
  //Destructor
  ~Pfile();

  //Add a file
  //Remove a file
  
  //info
  void info(const Pworld& pworld, Pprint& pprint) const;

  //recover
  
  //Open all files
  //Close all files
  //Open a file
  //Close a file
  //check if file is open and with what status

};


#endif
