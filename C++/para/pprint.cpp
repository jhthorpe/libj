/*--------------------------------------------------------
  pprint.cpp
	JHT, Febuary 7, 2022 : created


  .cpp file for pprint, which stores (potentially parallel)
  print buffers
--------------------------------------------------------*/
#include "para.hpp"

void Pprint::print_all(const Pworld& pworld) const
{
  //MPI code
  #if defined LIBJ_MPI
  for (int message=0;message<vec.size;message++)
  {
    //gather messages
    MPI_Barrier(pworld.comm_world);

    const int slen = strlen(vec[message]);
    MPI_Gather(vec[message],
               slen,MPI_CHAR,
               pbuffer+PPRINT_LEN*pworld.mpi_task_id,
               PPRINT_LEN,MPI_CHAR,
               0,
               pworld.comm_world);

    //Print each message
    if (pworld.mpi_ismaster)
    {
      for (int task=0;task<pworld.mpi_num_tasks;task++)
      {
        printf("%s",pbuffer+PPRINT_LEN*task);
      }

    //clean the message buffer
    memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_num_tasks);
    }

  } 

  //Non-MPI code
  #else
  for (int message=0;message<vec.size;message++)
  {
    printf("%s",vec[message]);
    memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_num_tasks);
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

  const int slen = strlen(vec[message]);
  MPI_Gather(vec[message],
             slen,MPI_CHAR,
             pbuffer+PPRINT_LEN*pworld.mpi_task_id,
             PPRINT_LEN,MPI_CHAR,
             0,
             pworld.comm_world);

  //Print each message
  if (pworld.mpi_ismaster)
  {
    for (int task=0;task<pworld.mpi_num_tasks;task++)
    {
      printf("%s",pbuffer+PPRINT_LEN*task);
    }

  //clean the message buffer
  memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_num_tasks);
  }

  //Non-MPI code
  #else
  printf("%s",vec[message]);
  memset(pbuffer,(char)0,sizeof(char)*PPRINT_LEN*pworld.mpi_num_tasks);
  #endif
}
