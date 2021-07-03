/*-------------------------------------------------------
  fsys.hpp
	JHT, July 2, 2021

  (F)ile(SYS)tem
    - initializes the filestystem class, which tracks
      binary files 

-------------------------------------------------------*/
#ifndef FSYS_HPP
#define FSYS_HPP

#include <cstdlib>  //for size_t
#include <stdio.h>  //for printf
#include <vector>   //for vector 
#include <string>   //for string

class fsys
{
private:
  std::vector <std::size_t> nbyte;  //next unwritten byte
  std::vector <FILE*>        fptr;  //file pointers
  std::vector <std::string> fname;  //file names
  std::vector <std::string> fmode;  //file modes
  std::vector <bool>       isopen;  //file open bool 
  std::vector <bool>       isfree;  //this file_id is free 
  std::vector <bool>        isbin;  //file is binary
  std::size_t	           nfiles;  //number of files
  std::size_t               nfree;  //number of free files

public:
  //Initialization and destruction
  fsys();
 ~fsys();

  //Adding and Remove files
  std::size_t add(const std::string fname);				//add file to system, return file id
  std::size_t remove(const std::string fname);				//add file to system, return file id
  std::size_t add(const std::string fname,const std::string fmode);	//add file to system, return file id

  //File open and closing
  void open(const std::string, const std::string fmode); 	//open based on file name
  void close(const std::string fname);		  		//close based on file name
  void open(const std::size_t fid, const std::string fmode);	//open based on file id
  void close(const std::size_t fid);				//close based on file id
  void close_all();						//closes all files
  void open_all();						//opens all files

  //File system saving and recovering
  void save();							//saves the filesystem
  void recover();						//recovers the filesystem
   
};
#endif
