/*------------------------------------------------------------------------
 * pfile.hpp
 *  JHT, Febuary 8, 2022: created
 *
   .hpp file for Pfile, which handles a (possibly parallel) filesystem
   Also contains the PFIO struct, which 
 
  In the following, there are three kinds of member functions. Those that
    take in an ``external'' name, which is converted into an internal 
    id, are marked with "s" to start. Internal functions, which generally
    only take internal file ids, either start with no extra letter, or the 
    letter "x" to indicate that they do not check if this mpi task is in 
    charge of IO. 
 
   NOTE : seek,get_pos,write, and read do *NOT* have an internal safety 
          wrapper for checking if the MPI task is in charge of file IO, 
          due to the need for speed in these functions 
 
  Init must be called after construction

  General usage

  The external name subroutines, those that begin with "s", are safest.
  The internal id subroutines, which take pworld as a parameter, can be
    called by any MPI task (as can the "s" subroutines).
  The "unsafe" subroutines, those that begin with "x", and write,read,
    seek, get_pos, should only be called by a task which does the IO. 
 
------------------------------------------------------------------------*/
#ifndef LIBJ_PFILE_HPP
#define LIBJ_PFILE_HPP
#include "strvec.hpp"
#include "pprint.hpp"
#include "pworld.hpp"
#include <vector>
#include <stdio.h>
/*
 * Plain old data for file pointer and position
*/
#ifndef PFIO_HPP
#define PFIO_HPP
struct Pfio
{
  FILE* fptr; //file pointer
  long  fpos; //file 
};
#endif

/*
 * Pbool plain old data to ``cheat'' std::vector<bool>
*/
#ifndef PBOOL_HPP
#define PBOOL_HPP
struct Pbool
{
  bool val; //value
};
#endif 

#include "libjdef.h"

//Error message integers
#define PFILE_ERR_NULL -1 //for if opening file gave null pointer
#define PFILE_ERR_OPEN -2 //for if file was already open
#define PFILE_ERR_CLOSE -3 //for if file close failed
#define PFILE_ERR_ERASE -4 //for if file erase failed
#define PFILE_ERR_FLUSH -5 //could not flush file io buffer
#define PFILE_ERR_SLEN -6 //input string is too long
#define PFILE_RES 50
#define PFILE_LEN 32 //pfile max length of strvec

class Pfile
{
  private: 
  //Data
  std::vector<Pfio>        m_fio;	//file io struct list
  std::vector<Pbool>       m_isopen;	//bools for tracking if file is open
  Strvec<PFILE_LEN>        m_fname;	//file names
  Strvec<PFILE_LEN>        m_fstat;	//file status
  char                     m_buf[PFILE_LEN];
  int                      m_nfiles;  //number of files
  int                      m_rootid;  //root file id

  public:
  //Constructor/destructor
  Pfile();
  ~Pfile();

  //Init
  int init(const Pworld& pworld);

  //Add a file
  // Note that both sadd and add return the file id (or -val on error)
  // add needs the *internal* file name
  int sadd(const Pworld& pworld, const char* fname);
  int add(const Pworld& pworld, const char* fname); 
  int xadd(const char* fname); 

  //Remove a file
  // Note that sadd returns the file id (or -val on error)
  int sremove(const Pworld& pworld, const char* fname);
//  int remove(const Pworld& pworld, const int fid); 
  int xremove(const int fid); 

  //Check if file is open
  bool sisopen(const Pworld& pworld, const char* fname);
  bool isopen(const Pworld& pworld, const int fid) const; 
  bool xisopen(const int fid) const; 

  //Open a file
  //saddopen returns the internal file id on successful exit, and error on not 
  int sopen(const Pworld& pworld, const char* fname, const char* fstat);
  int saddopen(const Pworld& pworld, const char* fname, const char* fstat);
  int open(const Pworld& pworld, const int fid, const char* fstat); 
  int xopen(const int fid, const char* fstat); 
  
  //Close a file
  int sclose(const Pworld& pworld, const char* fname);
  int close(const Pworld& pworld, const int fid);
  int xclose(const int fid);
  int close_all(const Pworld& pworld);
  int xclose_all();

  //erase files -- close, delete, remove
  int serase(const Pworld& pworld, const char* fname);
  int erase(const Pworld& pworld, const int fid);
  int xerase(const int fid);
  int erase_all(const Pworld& pworld); 

  //Flush file buffer
  int sflush(const Pworld& pworld, const char* fname);
  int flush(const Pworld& pworld, const int fid) const;

  //Get file id within fsys
  int sget_fid(const Pworld& pworld, const char* fname);

  //locate file id from string
  int file_loc(const Pworld& pworld, const char* fname) const;
  int xfile_loc(const char* fname) const;
  
  //Make file name with world task id
  int make_name(const Pworld& pworld, const char* fname);
  
  //info
  void info(const Pworld& pworld, Pprint& pprint) const;

  //write : needs internal file id!!
  void write(const int fid, const long pos, const void* data,
             const size_t size, const size_t num);

  //read : needs internal file id!!
  void read(const int fid, const long pos, void* data,
            const size_t size, const size_t num);

  //seek : needs internal file id!!
  void seek(const int fid, const long pos);

  //get_pos : get file position
  long get_pos(const int fid) const {return m_fio[fid].fpos;}

  //save filesystem info
  int save(const Pworld& pworld);
   
  //recover filesystem info
  int recover(const Pworld& pworld);

};


#endif
