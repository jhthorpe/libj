/*------------------------------------------------------------------------
 * pfile.cpp
 *  JHT, Febuary 8, 2022 : created
 *
 *  .hpp file for Pfile, which handles a (possibly parallel) filesyste,
------------------------------------------------------------------------*/
#include "para.hpp"

//-----------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------
Pfile::Pfile()
{
  m_isopen.reserve(PFILE_RES);
  m_isbin.reserve(PFILE_RES);
  m_fptr.reserve(PFILE_RES);
  m_fpos.reserve(PFILE_RES);
  m_fname.reserve(PFILE_RES);
  m_fstat.reserve(PFILE_RES);
  m_nfiles = 0;
}

//-----------------------------------------------------------------------
// Destructor -- close all files
//-----------------------------------------------------------------------
Pfile::~Pfile()
{
  for (int file=0;file<m_nfiles;file++)
  {
    if (m_isopen[file]){fclose(m_fptr[file]);}
  }
}

//-----------------------------------------------------------------------
// Info
//-----------------------------------------------------------------------
void Pfile::info(const Pworld& pworld, Pprint& pprint) const
{
  pprint.store();
  if (pworld.mpi_ismaster) {pprint.add("\nPfile system\n");}
  pprint.add("Number of files on task %d\n",pworld.mpi_task_id);
  for (int file=0;file<m_nfiles;file++)
  {
    pprint.add("%s ",m_fname[file].c_str());
    pprint.add("\n");
  }
  pprint.store();
}
