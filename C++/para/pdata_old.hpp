/*------------------------------------------------------------------------
  pdata.hpp
   JHT, Febuary 7, 2022 : created
 
   .hpp file for Pdata, which manages a (possibly parallel) set of data. 
    This class manages the reading and writing of this data to and from 
    the Pfile system
 
  Data is primarily identified by two longs. First is the "list", which
  is a list of the same data types. There is then an index of each element
  of that list, for specific instances of those data types 

  Note that is is *not* assumed that lists are stored sequentially in 
  memory or in Pfile. However, all indexes of a particular list *are* 
  stored with the same external file name in "Pfile". 

  
  The most commonly used functions are
	- checking if data is in core or on disk
	- get pointer to data
	- pass data to/from tasks
	- read/write data from/to file
  
  Data has the following attributes
	- list   (list which this data belongs to)
 	- index  (index of this data within list)
	- size   (number of elements) 
	- incore (data is currently in core memory)
	
  Note that, if we code things intelligently, we will never ask for a
    value that isn't incore, so a hash-table should be a faster 
    approach

------------------------------------------------------------------------*/
#ifndef PDATA_HPP
#define PDATA_HPP

#include "libjdef.h"
#include <vector>

//------------------------------------------------------------------------
// Pdata_id
//    identifiers for unique data
//------------------------------------------------------------------------
struct Pdata_id
{
  //Data
  long list;
  long index;

  //comparison
  bool operator==(const Pdata_id& other) const;
  bool operator!=(const Pdata_id& other) const;
};

bool Pdata_id::operator==(const Pdata_id& other) const
{
  return (other.index == index && other.list == list);
}
bool Pdata_id::operator!=(const Pdata_id& other) const
{
  return (other.index != index || other.list != list);
}

//------------------------------------------------------------------------
// Pdata_fio
//	struct of paramaters needed for file IO with Pfile
//------------------------------------------------------------------------
struct Pdata_fio
{
  //data
  long file_id;
  long task_id;
};

//------------------------------------------------------------------------
// Pdata_struct
//------------------------------------------------------------------------
template <typename T>
struct Pdata_struct
{
  //Data
  std::vector<std::vector<T>> m_data;

  //Functions
  
};

//------------------------------------------------------------------------
// Pdata
//------------------------------------------------------------------------
#define PDATA_NAME_LEN 32
class Pdata
{
  private:
  long m_num_lists;
 
  //list of names
  Strvec<PDATA_NAME_LEN>  m_list_names;

  //File ids, tasks, and offsets, and sizes
  Pdata_files m_files;

  public:

  //Add a list with id and name
  long add_list(const char* name);
  long get_listid(const char* name);

};

//------------------------------------------------------------------------
// add_list
// 	adds a list with list_name, which is stored in file file_name	
//
//------------------------------------------------------------------------
long Pdata::add_list(const Pworld& pworld, Pfile& pfile, 
                     const char* list_name, const char* file_name)

{
  long list_id = get_listid(name);

  //list doesn't exist add it
  if (list_id == -1) 
  {
    //add name to list
    m_list_names.push_back(name); 
    list_id = (long) (m_list_names.size - 1);

    //add this file to the filesystem
    long fid = pfile.sadd(pworld,name);
    if (fid < 0) return -1;

    //add list file data
    m_

  //list exists
  } 
  return list_id;
}

//------------------------------------------------------------------------
// get_listid
//	Returns the list id if it exists
//------------------------------------------------------------------------
long Pdata::get_listid(const char* name)
{
  const long fid = (long) m_list_names.find_index(name);
  return fid;
} 

#endif
