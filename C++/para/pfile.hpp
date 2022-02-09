/*------------------------------------------------------------------------
 * pfile.hpp
 *  JHT, Febuary 8, 2022: created
 *
 *  .hpp file for Pfile, which handles a (possibly parallel) filesystem
 *  Also contains the PFIO struct, which 
------------------------------------------------------------------------*/

/*
 * Plain old data for fiel pointer and position
*/
#ifndef PFIO_HPP
#define PFIO_HPP
#include <stdio.h>
#include "libjdef.h"
struct Pfio
{
  FILE* fptr; //file pointer
  long  fpos; //file 
};
#endif

#ifndef PFILE_HPP
#define PFILE_HPP
#include <string>
#include <vector>
#include <stdio.h>
#include "libjdef.h"

//Error message integers
#define PFILE_ERR_NULL 1 //for if opening file gave null pointer
#define PFILE_ERR_OPEN 2 //for if file was already open
#define PFILE_ERR_CLOSE 3 //for if file close failed
#define PFILE_ERR_ERASE 4 //for if file erase failed
#define PFILE_RES 50

class Pfile
{
  private: 
  //Data
  std::vector<Pfio>        m_fio;     //file io struct list
  std::vector<bool>        m_isopen;  //bools for tracking if file is open
  std::vector<std::string> m_fname;   //file names
  std::vector<std::string> m_fstat;   //file status
  int                      m_nfiles;  //number of files

  public:
  //Initialize and destroy
  Pfile();
  ~Pfile();

  //Add a file
  int add(const Pworld& pworld, std::string fname);
  int sadd(const std::string& fname); //add if we already know name/loc

  //Remove a file
  int remove(const Pworld& pworld, std::string fname);
  int sremove(const int loc); //already have name/loc

  //Open a file
  int open(const Pworld& pworld, std::string fname, const std::string fstat);
  int sopen(const int loc, const std::string& fstat); 
  int addopen(const Pworld& pworld, std::string fname, const std::string fstat);
  
  //Close a file
  int close(const Pworld& pworld, std::string fname);
  int sclose(const int loc);
  int close_all();

  //erase files -- close, delete, remove
  int erase(const Pworld& pworld, std::string fname);
  int serase(const int loc);
  int erase_all(); 

  //locate file id from string
  int file_loc(const std::string fname) const;
  
  //Make file name with tasks
  void make_name(const Pworld& pworld, std::string& fname) const;
  
  //info
  void info(const Pworld& pworld, Pprint& pprint) const;

  //save filesystem info
  //recover filesystem info
  
};


#endif
