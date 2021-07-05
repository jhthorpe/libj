/*-------------------------------------------------------
  fsys.hpp
	JHT, July 2, 2021

  (F)ile(SYS)tem
    - initializes the filestystem class, which tracks
      binary files 

  NOTE : files can only be added to the system, I have
         not yet coded a good way of deleting them

  INITIALIZATION
  --------------------------
  fsys fs;				//empty initialization 

  ADD FILE
  --------------------------
  fs.add("file_name");			//adds file via string

  PRINT FSYS
  --------------------------
  fs.print();				//prints
 
  OPEN and CLOSE
  --------------------------
  fs.open("file_name","file_mode");	//opens file with a particular mode
  fs.close("file_name");		//closes file 
  fs.open_all();			//opens all files in the system
  fs.close_all();			//closes all files in the system

  SAVE or RECOVER FSYS
  --------------------------
  fs.save();				//saves current states of fsys to fsys.save
  fs.recover();				//recovers fsys (including openning of files) from fsys.save

  SET INFO
  --------------------------
  fs.set_mode("file_name","file_mode");	//sets mode of opening/accessing file

  GET INFO
  --------------------------
  fs.get_fptr("file_name");		//returns FILE* pointer
  fs.get_fid("file_name");		//returns file id (long)

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
  std::vector <long>         next;  //next unwritten location
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

  //Get and set
  long get_fid(const std::string fname) const;
  FILE* get_fptr(const std::string fname) const;
  void set_mode(const std::string fname, const std::string fmode); 

  //File open and closing
  void open(const std::string fname, const std::string fmode); 		//open based on file name
  void close(const std::string fname);		  			//close based on file name
  void close_all();							//closes all files
  void open_all();							//opens all files

  //File system saving and recovering
  void save() const;							//saves the filesystem
  void recover();							//recovers the filesystem
   
};
#endif
