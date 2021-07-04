/*-------------------------------------------------------
  fsys.hpp
	JHT, July 2, 2021

  (F)ile(SYS)tem
    - initializes the filestystem class, which tracks
      binary files 

  NOTE : files can only be added to the system, I have
         not yet coded a good way of deleting them

-------------------------------------------------------*/
#ifndef FSYS_HPP
#define FSYS_HPP

#include <cstdlib>  //for malloc
#include <stdio.h>  //for printf
#include <vector>   //for vector 
#include <string>   //for string

class fsys
{
private:
  std::vector <long>                             next;  //next unwritten location
  std::vector <FILE*>        fptr;  //file pointers
  std::vector <std::string> fname;  //file names
  std::vector <std::string> fmode;  //file modes
  std::vector <bool>       isopen;  //file open bool 
  long	                   nfiles;  //number of files
  long                      nfree;  //number of free files

public:
  //Initialization and destruction
  fsys();
 ~fsys();

  //Printing the system
  void print() const;

  //Adding and Remove files
  long add(const std::string fname);					//add file to system, return file id

  //File ID 
  long fid(const std::string) const;

  //File open and closing
  void open(const std::string, const std::string fmode); 		//open based on file name
/*
  void close(const std::string fname);		  			//close based on file name
  void open(const long fid, const std::string fmode);			//open based on file id
  void close(const long fid);						//close based on file id
  void close_all();							//closes all files
  void open_all();							//opens all files

  //File system saving and recovering
  void save();								//saves the filesystem
  void recover();							//recovers the filesystem
*/
   
};
#endif
