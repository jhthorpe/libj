/*----------------------------------------------------------------------------
  pdata.cpp
	JHT, Febuary 14, 2022 : created

  .cpp file for Pdata class
----------------------------------------------------------------------------*/
#include "pdata.hpp"

//----------------------------------------------------------------------------
// Pdata() constructor
//----------------------------------------------------------------------------
Pdata::Pdata()
{
  m_num_lists=0;
}
//----------------------------------------------------------------------------
// list_size
//----------------------------------------------------------------------------
long Pdata::list_bytes(const long list_id) const
{
  long bytes = 0;
  const long byteso = m_list_info[list_id].m_bytes;
  for (long index=0;index<m_list_size[list_id];index++)
  {
    bytes += byteso*m_index[list_id][index].m_size; 
  }
  return bytes;
}
//----------------------------------------------------------------------------
// add_index
//----------------------------------------------------------------------------
void Pdata::add_index(const long list_id, const int task_id, 
                     const long file_pos, const long index_size)
{
  m_list_size[list_id]++;
  m_index[list_id].push_back({task_id,file_pos,index_size});
}

//----------------------------------------------------------------------------
// get_file_id
//	Given the internal list_id, return the file_id
//----------------------------------------------------------------------------
int Pdata::get_file_id(const long list_id) const
{
  return m_list_info[list_id].m_file_id;
}

//----------------------------------------------------------------------------
// print lists
//----------------------------------------------------------------------------
void Pdata::print_lists(const Pworld& pworld, Pprint& pprint) const
{
  if (pworld.mpi_world_ismaster)
  {

    std::vector<long> bytes;
    bytes.reserve(m_num_lists);
    for (long list=0;list<m_num_lists;list++)
    {
      bytes.push_back(list_bytes(list));
    }

    pprint.add("Pdata lists\n");
    pprint.add("  List       Tag      File      Size\n");
    for (long list=0;list<m_num_lists;list++)
    {
      pprint.add("%6ld    %6ld    %6d      ",
                 list,m_list_tags[list],m_list_info[list].m_file_id);

      //print bytes
      if (bytes[list] < 1000)
      {
        pprint.add("%.2f B\n",(float) bytes[list]); 
      } else if (bytes[list] < 1000000) {
        pprint.add("%.2f KB\n",(float) bytes[list]/1000);
      } else if (bytes[list] < 1000000000) {
        pprint.add("%.2f MB\n",(float)bytes[list]/1000000);
      } else {
        pprint.add("%.2f GB \n",(float)bytes[list]/1000000000);
      } 

    }
  } 
  pprint.store();
}

//----------------------------------------------------------------------------
// Pdata::find_list
//      given an external list_tag, find the list_id
//----------------------------------------------------------------------------
long Pdata::find_list(const long list_tag) const
{
  for (long i=0;i<m_num_lists;i++)
  {
    if (m_list_tags[i] == list_tag) {return i;}
  }
  return -1;
}

//----------------------------------------------------------------------------
// Pdata::add_list
//      adds a new list based on the file_id which stores it's data, and the
//      number of bytes one element of one index of the list takes up
//----------------------------------------------------------------------------
long Pdata::add_list(const long list_tag, const int file_id,
                     const std::size_t bytes)
{ 
  //find tag 
  const long list_id = find_list(list_tag);
  
  //If file doesn't exist
  if (list_id == -1)
  { 
    m_num_lists++;
    m_list_tags.push_back(list_tag);
    m_list_size.push_back(0);
    m_list_info.push_back({file_id,bytes});
    m_index.resize(m_num_lists);
    return m_num_lists-1;
  //list does exist, and is the same
  } else if (list_id >= 0 
             && m_list_info[list_id].m_file_id == file_id
             && m_list_info[list_id].m_bytes == bytes){
    return list_id; 
  //list does exist, but is not the same, return error
  } else { 
    return -1;
  }

  return 0;
}

