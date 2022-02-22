/*----------------------------------------------------------------------------
  pdata.hpp
	JHT, Febuary 13, 2022 : created

  .hpp file for pdata class, which manages lists of data

  Data of the same kind is grouped into a "list", with individual "indexes"
    as specific instances of this data. 

  For example. T2 amplitudes of a particular DPD *could* be a list

  Parallization
  ---------------------
  - each task has a complete picture of all data needed, and which task is
    responsible for which index. We assume that the list structure isn't going
    to be changing during compute heavy routines, so that the memory and synch
    overhead between threads isn't so much of an issue
----------------------------------------------------------------------------*/
#ifndef LIBJ_PDATA_HPP
#define LIBJ_PDATA_HPP

#include <vector>
#include "libjdef.h"

#include "pworld.hpp"
#include "pfile.hpp"
#include "pprint.hpp"

//----------------------------------------------------------------------------
// Plist_info
//	file_id is the Pfile internal id which holds the task
 //	bytes is the number of bytes of one element of an index of the list 
//----------------------------------------------------------------------------
struct Plist_info
{
  int        m_file_id;
  std::size_t m_bytes;
};

//----------------------------------------------------------------------------
// Pindex_info
//	m_storage_task	which task is in charge of storing this index
//	m_file_pos	location of this index in the relevant file	
//	m_size		number of elements
//----------------------------------------------------------------------------
struct Pindex_info
{
  int  m_storage_task;
  long m_file_pos;
  long m_size;
};

//----------------------------------------------------------------------------
// Pdata class
//
// list_info tracks file ids and element bytes for each list
// list_size tracks the number of indexes of each list
// list_tags  stores an external tag of a particular list 
//----------------------------------------------------------------------------
class Pdata
{
  private:
  long                    m_num_lists;
  std::vector<long>       m_list_tags;
  std::vector<long>       m_list_size; 
  std::vector<Plist_info> m_list_info;

  std::vector<std::vector<Pindex_info>> m_index;

  public:

  Pdata();
  
  //adds a new list, returns the list_id
  long add_list(const long list_tag, const int file_id, const std::size_t bytes);

  //locates the list index given list_tag
  long find_list(const long list_tag) const;

  //return the file index, given the internal list_id
  int get_file_id(const long list_id) const;

  //print lists
  void print_lists(const Pworld& pworld, Pprint& pprint) const;

  //add index for an internal list_id 
  void add_index(const long list_id, const int task_id, 
                 const long file_pos, const long index_size);

  long list_bytes(const long list_id) const;
};

#endif
