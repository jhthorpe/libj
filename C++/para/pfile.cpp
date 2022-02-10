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
// Init -- initialize 0-file, the file for this class
//-----------------------------------------------------------------------
int Pfile::init(const Pworld& pworld)
{
  m_rootid = sadd(pworld,"pfile"); 
  return 0;
}

//-----------------------------------------------------------------------
// issopen -- check if file is open given external name 
//-----------------------------------------------------------------------
bool Pfile::issopen(const Pworld& pworld, std::string fname) const
{
  make_name(pworld,fname);
  int loc = file_loc(fname);
  if (loc != -1)
  {
    return m_isopen[loc];
  } else {
    return false;
  }
}

//-----------------------------------------------------------------------
// isopen -- check if file is open given file id
//-----------------------------------------------------------------------
bool Pfile::isopen(const int fid) const
{
  if (fid >= 0 && fid < m_nfiles)
  {
    return m_isopen[fid];
  } else {
    return false;
  }
}
//-----------------------------------------------------------------------
// Info
//-----------------------------------------------------------------------
void Pfile::info(const Pworld& pworld, Pprint& pprint) const
{
 
  //only the MASTER on the shared does any IO
  if (pworld.mpi_shared_ismaster) 
  {
    pprint.add("\nPfile system\n");

    //NOTE -- we use the WORLD task id as this is unique
    pprint.add("Number of files on task %d : %d\n",
               pworld.mpi_world_task_id,m_nfiles);

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

  //Add an empy message for all the shared tasks that aren't master
  } else {
    pprint.addstore("%c",(char)0);
    //pprint.addstore("%c",(char)0);
  }
}

//-----------------------------------------------------------------------
// Add -- add a string to the filesystem 
//-----------------------------------------------------------------------
int Pfile::sadd(const Pworld& pworld, std::string fname)
{
  //look for file in list
  make_name(pworld,fname);
  int loc = file_loc(fname);
  if (loc == -1) {loc = add(fname);} 
  return loc;
}

//-----------------------------------------------------------------------
// sadd -- add file, given we already have name and checked if it exists 
//-----------------------------------------------------------------------
int Pfile::add(const std::string& fname)
{
  m_nfiles++;
  m_isopen.push_back(false);  
  m_fio.push_back({NULL,0});
  m_fname.push_back(fname);
  m_fstat.push_back("c");
  return m_nfiles-1; 
}

//-----------------------------------------------------------------------
// sremove -- remove a file from the filesystem given it's unadjusted name
//-----------------------------------------------------------------------
int Pfile::sremove(const Pworld& pworld, std::string fname)
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
// remove -- remove a file from the filesystem based on file id
//-----------------------------------------------------------------------
int Pfile::remove(const int fid)
{
  if (m_fio[fid].fptr != NULL) {fclose(m_fio[fid].fptr);}
  m_fio.erase(m_fio.begin()+fid);
  m_isopen.erase(m_isopen.begin()+fid);
  m_fname.erase(m_fname.begin()+fid);
  m_fstat.erase(m_fstat.begin()+fid);
  m_nfiles--;
  return 0;
}

//-----------------------------------------------------------------------
// open -- opens a file with external name
//-----------------------------------------------------------------------
int Pfile::sopen(const Pworld& pworld, std::string fname, const std::string fstat)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);

  if (loc != -1)
  {
    stat = open(loc,fstat);
  } else {
    stat = PFILE_ERR_OPEN;
  }
  return stat;
}

//-----------------------------------------------------------------------
// saddopen -- add and open a file file with external name 
//   returns the internal file id of the file, or an error message if it failed
//-----------------------------------------------------------------------
int Pfile::saddopen(const Pworld& pworld, std::string fname, const std::string fstat)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);

  //If file doesn't exist
  if (loc == -1)
  {
    stat = add(fname); 
    if (stat != 0) return stat;
    loc = m_nfiles-1;
    stat = open(loc,fstat);    
 
  //file exists, error
  } else {
    stat = PFILE_ERR_OPEN;
  }

  return stat; 
}
//-----------------------------------------------------------------------
// open -- open with with internal id 
//-----------------------------------------------------------------------
int Pfile::open(const int fid, const std::string& fstat) 
{
  int stat=0;

  if (fid >= 0 && fid < m_nfiles)
  {

    //if file is closed  
    if (!m_isopen[fid])
    {
      m_fio[fid].fptr = fopen(m_fname[fid].c_str(),fstat.c_str());

      //check for successful open
      if (m_fio[fid].fptr != NULL)
      {
        m_fio[fid].fpos = 0;
        m_isopen[fid] = true;
        m_fstat[fid] = fstat;

      //bad open
      } else {
        stat = PFILE_ERR_NULL; 
      }     

    //File is open
    } else {
      stat = PFILE_ERR_OPEN;
    }
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
      stat += close(file);
    }
  };
  return (stat == 0) ? 0 : PFILE_ERR_CLOSE;
}

//-----------------------------------------------------------------------
// sclose -- close file with external name 
//-----------------------------------------------------------------------
int Pfile::sclose(const Pworld& pworld, std::string fname)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);
  if (loc != -1)
  {
    stat = close(loc); 
  } else {
    stat = PFILE_ERR_CLOSE; 
  }
  return stat;
}

//-----------------------------------------------------------------------
// close -- close file with internal id 
//-----------------------------------------------------------------------
int Pfile::close(const int fid)
{
  int stat = 0;

  //if file is open 
  if (m_isopen[fid])
  {
    stat = fclose(m_fio[fid].fptr);
    m_fio[fid].fptr = NULL;
    m_fio[fid].fpos = 0;
    m_isopen[fid] = false;
    m_fstat[fid] = "c";
  
    if (stat != 0) {stat = PFILE_ERR_CLOSE;}

  //File is closed
  } else {
    stat = PFILE_ERR_CLOSE;
  }

  return stat;
}

//-----------------------------------------------------------------------
// serase file with external name
//-----------------------------------------------------------------------
int Pfile::serase(const Pworld& pworld, std::string fname)
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);

  //if file is in filesystem
  if (loc != -1)
  {
    stat = erase(loc);
  //file is not in filesystem
  } else {
    stat = PFILE_ERR_ERASE;
  }

  return stat; 
}

//-----------------------------------------------------------------------
// erase file with internal location 
//-----------------------------------------------------------------------
int Pfile::erase(const int fid)
{
    int stat = close(fid);   
    //if (stat != 0) {return stat;}
    stat += std::remove(m_fname[fid].c_str()); //C remove function
    //if (stat != 0) {return PFILE_ERR_ERASE;}
    stat += remove(fid); //internal remove function
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
    stat += erase(file); 
  }
  return (stat == 0) ? 0 : PFILE_ERR_ERASE;
}

//-----------------------------------------------------------------------
// make_name -- makes the name given  
//-----------------------------------------------------------------------
void Pfile::make_name(const Pworld& pworld, std::string& fname) const
{
  fname.append(std::to_string(pworld.mpi_world_task_id));
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

//-----------------------------------------------------------------------
// write -- write bytes to file
//-----------------------------------------------------------------------
void Pfile::write(const int file, const long pos, const void* data, 
                  const size_t size, const size_t num)
{
  seek(file,pos); //this updates m_fio[file].fpos
  fwrite(data,size,num,m_fio[file].fptr);
  m_fio[file].fpos += (long) size*num;
}

//-----------------------------------------------------------------------
// read -- read bytes from file
//-----------------------------------------------------------------------
void Pfile::read(const int file, const long pos, void* data, 
                  const size_t size, const size_t num)
{
  seek(file,pos);
  fread(data,size,num,m_fio[file].fptr);
  m_fio[file].fpos += (long) size*num;
}
//-----------------------------------------------------------------------
// seek -- go to some position in a file, but check we are not already
//  there first
//-----------------------------------------------------------------------
void Pfile::seek(const int file, const long pos)
{
  if (pos != m_fio[file].fpos)
  {
    fseek(m_fio[file].fptr,pos,SEEK_SET); 
    m_fio[file].fpos = pos;
  }
}

//-----------------------------------------------------------------------
// sflush -- flush file buffer with external name
//-----------------------------------------------------------------------
int Pfile::sflush(const Pworld& pworld, std::string fname) const
{
  int stat = 0;
  make_name(pworld,fname);
  int loc = file_loc(fname);
  
  if (stat != -1)
  {
    fflush(m_fio[loc].fptr);
  } else {
    stat = PFILE_ERR_FLUSH; 
  }
  return stat;
}

//-----------------------------------------------------------------------
// flush -- flush with internal id 
//-----------------------------------------------------------------------
int Pfile::flush(const int fid) const
{
  int stat = 0;
  fflush(m_fio[fid].fptr);
  return stat;
}

//-----------------------------------------------------------------------
// get_fid
//-----------------------------------------------------------------------
int Pfile::get_fid(const Pworld& pworld, std::string fname) const
{
  make_name(pworld,fname); 
  int loc = file_loc(fname);
  return loc;
}

//-----------------------------------------------------------------------
// save 
//-----------------------------------------------------------------------
int Pfile::save(const Pworld& pworld) 
{
  int stat = 0;

  //Only the master in shared saves the data
  if (pworld.mpi_shared_ismaster)
  {
    //Open file
    if (isopen(m_rootid)) {stat = close(m_rootid);}
    if (stat != 0) {return stat;}
    if (open(m_rootid,"w+b") != 0) {return PFILE_ERR_OPEN;}  

    //write size
    long foff = 0;
    write(m_rootid,foff,&m_nfiles,sizeof(m_nfiles),1); 
    foff = get_pos(m_rootid);

    //Write data
    write(m_rootid,foff,m_fio.data(),sizeof(Pfio),m_nfiles); 
    foff = get_pos(m_rootid);

    //Close file
    if (close(m_rootid) != 0) {return PFILE_ERR_CLOSE;}
  }

  return stat;
}

