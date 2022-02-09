/*------------------------------------------------------------------------
 * pfile.hpp
 *  JHT, Febuary 8, 2022: created
 *
 *  .hpp file for Pfile, which handles a (possibly parallel) filesystem
 *  Also contains the PFIO struct, which 
 *
 * In the following, there are two kinds of member functions. Those that
 *   take in an ``external'' name, which is converted into an internal 
 *   id, and those that take just the internal id. 
 *
 *   The former are marked with an "s" at the start of the function name
 *   The latter are more efficient and should be used in general
 *
 * USAGE
 * ---------------
 * //To add files
 * const int fid = pfile.sadd(pworld,"filename"); 
 *
 * //To find the internal file id
 * const int fid = pfile.get_fid(pworld,"filename");
 *
 * //To remove a file from the list, but NOT delete it
 * stat = pfile.sremove(pworld,"filename"); 
 * stat = pfile.remove(file_id);
 *
 * //To open or close a file
 * stat = pfile.sopen(pworld,"filename","w+b");
 * stat = pfile.open(fid,"w+b"); 
 * stat = pfile.sclose(pworld,"filename");
 * stat = pfile.close(fid);
 * stat = close_all();
 *
 * //To add a file and open it
 * //Note that fid >= 0 is the file_id, and fid < 0 is error message
 * const int fid = pfile.saddopen(pworld,"filename","w+b");
 *
 * //to remove a file from both the filesystem and disk
 * stat = pfile.serase(pworld,"filename");
 * stat = pfile.erase(file_id);
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
#define PFILE_ERR_NULL -1 //for if opening file gave null pointer
#define PFILE_ERR_OPEN -2 //for if file was already open
#define PFILE_ERR_CLOSE -3 //for if file close failed
#define PFILE_ERR_ERASE -4 //for if file erase failed
#define PFILE_ERR_FLUSH -5 //could not flush file io buffer
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
  // Note that both sadd and add return the file id (or -1 on error)
  int sadd(const Pworld& pworld, std::string fname);
  int add(const std::string& fname); //add if we already know name/loc

  //Remove a file
  int sremove(const Pworld& pworld, std::string fname);
  int remove(const int fid); //already have name/loc

  //Open a file
  //saddopen returns the internal file id on successful exit, and error on not 
  int sopen(const Pworld& pworld, std::string fname, const std::string fstat);
  int saddopen(const Pworld& pworld, std::string fname, const std::string fstat);
  int open(const int fid, const std::string& fstat); 
  
  //Close a file
  int sclose(const Pworld& pworld, std::string fname);
  int close(const int fid);
  int close_all();

  //erase files -- close, delete, remove
  int serase(const Pworld& pworld, std::string fname);
  int erase(const int fid);
  int erase_all(); 

  //Flush file buffer
  int sflush(const Pworld& pworld, std::string fname) const;
  int flush(const int fid) const;

  //Get file id within fsys
  int get_fid(const Pworld& pworld, std::string fname) const;

  //locate file id from string
  int file_loc(const std::string fname) const;
  
  //Make file name with tasks
  void make_name(const Pworld& pworld, std::string& fname) const;
  
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

  //save filesystem info
  //recover filesystem info
  
};


#endif
