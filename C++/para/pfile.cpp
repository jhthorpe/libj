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
  m_fio.reserve(PFILE_RES);
  m_isopen.reserve(PFILE_RES);
  m_fname.reserve(PFILE_RES);
  m_fstat.reserve(PFILE_RES);
  m_nfiles = 0;
}

//-----------------------------------------------------------------------
// Destructor -- close all files
//-----------------------------------------------------------------------
Pfile::~Pfile()
{
  //close all in the future
  close_all();
}

//-----------------------------------------------------------------------
// Info
//-----------------------------------------------------------------------
void Pfile::info(const Pworld& pworld, Pprint& pprint) const
{
  pprint.store();
  if (pworld.mpi_ismaster) {pprint.add("\nPfile system\n");}
  pprint.add("Number of files on task %d : %d\n",pworld.mpi_task_id,m_nfiles);
  if (m_nfiles > 0)
  {
    pprint.add("File            Open      Status\n");
    for (int file=0;file<m_nfiles;file++)
    {
 
      //print first 16 characters of name
      const int len = m_fname[file].length();
      const int mm = (len <= 16) ? len : 16;
      for (int c=0;c<mm;c++)
      {
        pprint.add("%c",m_fname[file][c]);
      }
      for (int c=len;c<16;c++)
      {
        pprint.add(" ");
      }

      if (m_isopen[file])
      {
        pprint.add("t          %s\n",m_fstat[file].c_str());
      } else {
        pprint.add("f\n");
      }
    }
  }
  pprint.store();
}

//-----------------------------------------------------------------------
// Add -- add a file to the system  
//-----------------------------------------------------------------------
int Pfile::add(const Pworld& pworld, std::string fname)
{
  //look for file in list
  make_name(pworld,fname);
  int loc = file_loc(fname);
  if (loc == -1) 
  {
    m_nfiles++;
    m_fio.push_back({NULL,0});
    m_isopen.push_back(false);  
    m_fname.push_back(fname);
    m_fstat.push_back("c");
  } 
  return (loc == -1) ? 0 : 1;
}

//-----------------------------------------------------------------------
// sadd -- add a file to the system if we've already fixed name 
//-----------------------------------------------------------------------
int Pfile::sadd(const std::string& fname)
{
  m_nfiles++;
  m_isopen.push_back(false);  
  m_fio.push_back({NULL,0});
  m_fname.push_back(fname);
  m_fstat.push_back("c");
  return 0; 
}

//-----------------------------------------------------------------------
// remove -- remove a file from the filesystem
//-----------------------------------------------------------------------
int Pfile::remove(const Pworld& pworld, std::string fname)
{
  make_name(pworld,fname);
  int loc = file_loc(fname);
  
  if (loc != -1)
  {
    if (m_fio[loc].fptr != NULL) {fclose(m_fio[loc].fptr);}

    m_fio.erase(m_fio.begin()+loc);
    m_isopen.erase(m_isopen.begin()+loc);
    m_fname.erase(m_fname.begin()+loc);
    m_fstat.erase(m_fstat.begin()+loc);
    m_nfiles--;
  }

  return (loc != -1) ? 0 : 1;
}

//-----------------------------------------------------------------------
// sremove -- remove a file from the filesystem if we already have name
//-----------------------------------------------------------------------
int Pfile::sremove(const int loc)
{
  if (m_fio[loc].fptr != NULL) {fclose(m_fio[loc].fptr);}
  m_fio.erase(m_fio.begin()+loc);
  m_isopen.erase(m_isopen.begin()+loc);
  m_fname.erase(m_fname.begin()+loc);
  m_fstat.erase(m_fstat.begin()+loc);
  m_nfiles--;
  return 0;
}

//-----------------------------------------------------------------------
// open -- opens a file without knowing location
//-----------------------------------------------------------------------
int Pfile::open(const Pworld& pworld, std::string fname, const std::string fstat)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);

  if (loc != -1)
  {
    stat = sopen(loc,fstat);
  } else {
    stat = PFILE_ERR_OPEN;
  }
  return stat;
}

//-----------------------------------------------------------------------
// addopen -- add and open a file 
//-----------------------------------------------------------------------
int Pfile::addopen(const Pworld& pworld, std::string fname, const std::string fstat)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);

  //If file doesn't exist
  if (loc == -1)
  {
    stat = sadd(fname); 
    if (stat != 0) return stat;
    loc = m_nfiles-1;
    stat = sopen(loc,fstat);    
 
  //file exists, error
  } else {
    stat = PFILE_ERR_OPEN;
  }

  return stat;
}
//-----------------------------------------------------------------------
// sopen -- given a file's location in the system, opens it 
//-----------------------------------------------------------------------
int Pfile::sopen(const int loc, const std::string& fstat) 
{
  int stat=0;

  //if file is closed  
  if (!m_isopen[loc])
  {
    m_fio[loc].fptr = fopen(m_fname[loc].c_str(),fstat.c_str());

    //check for successful open
    if (m_fio[loc].fptr != NULL)
    {
      m_fio[loc].fpos = 0;
      m_isopen[loc] = true;
      m_fstat[loc] = fstat;

    //bad open
    } else {
      stat = PFILE_ERR_NULL; 
    }     

  //File is open
  } else {
    stat = PFILE_ERR_OPEN;
  }

  return stat;
}

//-----------------------------------------------------------------------
// close_all -- close all files 
//-----------------------------------------------------------------------
int Pfile::close_all()
{
  int stat = 0;
  for (int file=0;file<m_nfiles;file++) 
  {
    if (m_isopen[file])
    {
      stat += sclose(file);
    }
  };
  return (stat == 0) ? 0 : PFILE_ERR_CLOSE;
}

//-----------------------------------------------------------------------
// Close 
//-----------------------------------------------------------------------
int Pfile::close(const Pworld& pworld, std::string fname)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);
  if (loc != -1)
  {
    stat = sclose(loc); 
  } else {
    stat = PFILE_ERR_CLOSE; 
  }
  return stat;
}

//-----------------------------------------------------------------------
// sclose -- close file if location is already known
//-----------------------------------------------------------------------
int Pfile::sclose(const int loc)
{
  int stat = 0;

  //if file is open 
  if (m_isopen[loc])
  {
    stat = fclose(m_fio[loc].fptr);
    m_fio[loc].fptr = NULL;
    m_fio[loc].fpos = 0;
    m_isopen[loc] = false;
    m_fstat[loc] = "c";
  
    if (stat != 0) {stat = PFILE_ERR_CLOSE;}

  //File is closed
  } else {
    stat = PFILE_ERR_CLOSE;
  }

  return stat;
}

//-----------------------------------------------------------------------
// erase file with external name
//-----------------------------------------------------------------------
int Pfile::erase(const Pworld& pworld, std::string fname)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);

  //if file is in filesystem
  if (loc != -1)
  {
    stat = serase(loc);
  //file is not in filesystem
  } else {
    stat = PFILE_ERR_ERASE;
  }

  return stat; 
}

//-----------------------------------------------------------------------
// erase file with internal location 
//-----------------------------------------------------------------------
int Pfile::serase(const int loc)
{
    int stat = sclose(loc);   
    //if (stat != 0) {return stat;}
    stat += std::remove(m_fname[loc].c_str()); //C remove function
    //if (stat != 0) {return PFILE_ERR_ERASE;}
    stat += sremove(loc); //internal remove function
    return (stat == 0) ? 0 : PFILE_ERR_ERASE; 
}


//-----------------------------------------------------------------------
// erase_all -- erase all files on filesystem 
//-----------------------------------------------------------------------
int Pfile::erase_all()
{
  int stat = 0;
  for (int file=0;file<m_nfiles;file++)
  {
    stat += serase(file); 
  }
  return (stat == 0) ? 0 : PFILE_ERR_ERASE;
}

//-----------------------------------------------------------------------
// make_name -- makes the name given  
//-----------------------------------------------------------------------
void Pfile::make_name(const Pworld& pworld, std::string& fname) const
{
  fname.append(std::to_string(pworld.mpi_task_id));
}

//-----------------------------------------------------------------------
// file_loc -- find location of file by string 
//-----------------------------------------------------------------------
int Pfile::file_loc(const std::string fname) const
{
  int loc = -1;
  for (int file=0;file<m_nfiles;file++)
  {
    if (fname.compare(m_fname[file]) == 0) {return file;} 
  }
  return loc;
}
