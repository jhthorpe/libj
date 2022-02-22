/*--------------------------------------------------------
  pprint.cpp
	JHT, Febuary 7, 2022 : created


  .cpp file for pprint, which stores (potentially parallel)
  print buffers
--------------------------------------------------------*/
#include "pprint.hpp"
//--------------------------------------------------------
// Pprint initializer 
//--------------------------------------------------------
Pprint::Pprint()
{
  idx = 0;
  memset(buffer,(char)0,sizeof(char)*PPRINT_LEN);
  memset(stemp,(char)0,sizeof(char)*PPRINT_LEN);
  pbuffer = NULL;
}

//--------------------------------------------------------
// Pprint destructor
//--------------------------------------------------------
Pprint::~Pprint()
{
  if (pbuffer != NULL) free(pbuffer);
}

//--------------------------------------------------------
// Pprint initialize
//--------------------------------------------------------
int Pprint::init(const Pworld& pworld)
{
  int stat = 0;
  if (pworld.mpi_world_ismaster)
  {
    pbuffer = (char*) malloc(sizeof(char)*PPRINT_LEN*pworld.mpi_world_num_tasks);
    if (pbuffer != NULL)
    {
      stat = 0;
    } else {
      printf("\nERROR ERROR ERORR\n");
      printf("Pprint::init could not malloc %ld bytes\n",
             sizeof(char)*PPRINT_LEN*pworld.mpi_world_num_tasks);
      stat = 1;
    } 
  }          
  return stat;
}

//--------------------------------------------------------
// Pprint destroy
//--------------------------------------------------------
int Pprint::destroy(const Pworld& pworld)
{
  int stat = 0;
  if (pworld.mpi_world_ismaster)
  {
    if (pbuffer != NULL) free(pbuffer);
    pbuffer = NULL;
  } 
  return stat;
}

//--------------------------------------------------------
// Pprint add 
//--------------------------------------------------------
int Pprint::add(const char* fstring,...)
{
  int stat=0;
  //if buffer has no elements 
  if (idx == 0)
  {
    va_list arg;
    va_start(arg,fstring); 
    stat = vsprintf(buffer,fstring,arg);
    va_end(arg);
    idx = strlen(buffer);
  //buffer has elements, we need to concatinate
  } else {
    va_list arg;
    va_start(arg,fstring);
    stat = vsprintf(stemp,fstring,arg); 
    va_end(arg);
    int len = strlen(stemp);
    if (idx + len < PPRINT_LEN)
    {
      memmove(&buffer[idx],stemp,sizeof(char)*len);//move stemp elements
      memset(stemp,(char)0,sizeof(char)*len);//clear stemp elements
      idx += len;
    } else {
      printf("\nERROR ERROR ERROR\n");
      printf("Pprint::add string would case buffer overflow\n");
      return 1;
    }
  }
  return stat;
}

//--------------------------------------------------------
// Pprint vadd 
//--------------------------------------------------------
int Pprint::vadd(const char* fstring, va_list arg)
{
  int stat=0;
  //if buffer has no elements 
  if (idx == 0)
  { 
    stat = vsprintf(buffer,fstring,arg);
    idx = strlen(buffer);
  //buffer has elements, we need to concatinate
  } else {
    stat = vsprintf(stemp,fstring,arg);
    
    int len = strlen(stemp);
    if (idx + len < PPRINT_LEN)
    {
      memmove(&buffer[idx],stemp,sizeof(char)*len);//move stemp elements
      memset(stemp,(char)0,sizeof(char)*len);//clear stemp elements
      idx += len;
    } else {
      printf("\nERROR ERROR ERROR\n");
      printf("Pprint::add string would case buffer overflow\n");
      return 1;
    } 
  } 
  return stat;
}

//--------------------------------------------------------
// Pprint store
//--------------------------------------------------------
void Pprint::store()
{
  vec.push_back(buffer);
  clear();
}

//--------------------------------------------------------
// Pprint addstore
//--------------------------------------------------------
void Pprint::addstore(const char* fstring,...)
{
  va_list arg;
  va_start(arg,fstring);
  vadd(fstring,arg);
  va_end(arg);
  store();
}

//--------------------------------------------------------
// Pprint clear 
//--------------------------------------------------------
void Pprint::clear()
{
  memset(buffer,(char)0,sizeof(char)*idx);
  idx = 0;
}

//--------------------------------------------------------
// Pprint reset
//--------------------------------------------------------
void Pprint::reset()
{
  vec.clear();
  clear();
}

//--------------------------------------------------------
// print_all 
//--------------------------------------------------------
void Pprint::print_all(const Pworld& pworld) const
{
  //MPI code
  #if defined LIBJ_MPI
  for (int message=0;message<vec.size;message++)
  {
    //gather messages
    MPI_Barrier(pworld.comm_world);

    MPI_Gather(vec[message],
               PPRINT_LEN,MPI_CHAR,
               pbuffer+PPRINT_LEN*pworld.mpi_world_task_id,
               PPRINT_LEN,MPI_CHAR,
               0,
               pworld.comm_world);

    //Print each message
    if (pworld.mpi_world_ismaster)
    {
      for (int task=0;task<pworld.mpi_world_num_tasks;task++)
      {
        printf("%s",pbuffer+PPRINT_LEN*task);
      }

      //clean the message buffer
      memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_world_num_tasks);
    }

  } 

  //Non-MPI code
  #else
  for (int message=0;message<vec.size;message++)
  {
    printf("%s",vec[message]);
    memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_world_num_tasks);
  }
  #endif

}

//-------------------------------------------------------------------
// Print specific message
void Pprint::print(const Pworld& pworld, const int message) const
{

  //MPI code
  #if defined LIBJ_MPI
  //gather messages
  MPI_Barrier(pworld.comm_world);
  MPI_Gather(vec[message],
             PPRINT_LEN,MPI_CHAR,
             pbuffer+PPRINT_LEN*pworld.mpi_world_task_id,
             PPRINT_LEN,MPI_CHAR,
             0,
             pworld.comm_world);

  //Print each message
  if (pworld.mpi_world_ismaster)
  {
    for (int task=0;task<pworld.mpi_world_num_tasks;task++)
    {
      printf("%s",pbuffer+PPRINT_LEN*task);
    }

    //clean the message buffer
    memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_world_num_tasks);
  }

  //Non-MPI code
  #else
  printf("%s",vec[message]);
  memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_world_num_tasks);
  #endif
}
