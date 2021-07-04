/*-------------------------------------------------------
  fsys.hpp
	JHT, July 2, 2021

  (F)ile(SYS)tem
    - initializes the filestystem class, which tracks
      binary files 

-------------------------------------------------------*/
#ifndef FSYS_HPP
#define FSYS_HPP

#include <cstdint>  //for int64_t
#include <cstdlib>  //for malloc
#include <stdio.h>  //for printf
#include <vector>   //for vector 
#include <string>   //for string

class fsys
{
private:
  std::vector <int64_t>      next;  //next unwritten location
  std::vector <FILE*>        fptr;  //file pointers
  std::vector <std::string> fname;  //file names
  std::vector <std::string> fmode;  //file modes
//  std::vector <bool>       isopen;  //file open bool 
//  std::vector <bool>       isfree;  //this file_id is free 
  int64_t	           nfiles;  //number of files
  int64_t                   nfree;  //number of free files

public:
  //Initialization and destruction
  fsys();
 ~fsys();

  //Adding and Remove files
//  int64_t add(const std::string fname);					//add file to system, return file id
//  int64_t remove(const std::string fname);				//add file to system, return file id
//  int64_t add(const std::string fname,const std::string fmode);		//add file to system, return file id

  //Printing the system
//  void print() const;

  //File open and closing
/*
  void open(const std::string, const std::string fmode); 	//open based on file name
  void close(const std::string fname);		  		//close based on file name
  void open(const std::int64_t fid, const std::string fmode);	//open based on file id
  void close(const std::int64_t fid);				//close based on file id
  void close_all();						//closes all files
  void open_all();						//opens all files

  //File system saving and recovering
  void save();							//saves the filesystem
  void recover();						//recovers the filesystem
*/
   
};
#endif
