/*-----------------------------------------------------------------
  pworld.hpp
	JHT, Febuary 7, 2022 : created

  .hpp file for Pworld, which manages the initialization and 
  finalization of MPI parameters if they are required

//Usage
init()		: initializes variables and structures
destroy()	: finalizes variables and structures
info()		: prints info about the world

//Communicators
comm_world 	: MPI_COMM_WORLD

-----------------------------------------------------------------*/
#ifndef PWORLD_HPP
#define PWORLD_HPP
#include <stdio.h>

#include "libjdef.h"

#if defined LIBJ_MPI
  #include <mpi.h>
#endif

#if defined LIBJ_OMP
  #include <omp.h>
#endif

struct Pworld
{
  public:
  
  //Basic Data
  int mpi_num_tasks;
  int mpi_task_id;
  int omp_num_threads;
  bool ismpi;
  bool isomp;
  bool mpi_ismaster;

  //MPI communicators
  #if defined LIBJ_MPI
    MPI_Comm comm_world = MPI_COMM_WORLD;
    //Other communicators?
  #endif

  //Initializer
  int init()
  {
    #if defined LIBJ_MPI
      MPI_Init(NULL,NULL);
      MPI_Comm_size(comm_world,&mpi_num_tasks);
      MPI_Comm_rank(comm_world,&mpi_task_id);
      if (mpi_task_id == 0) {mpi_ismaster = true;}
      ismpi = true;
    #else
      mpi_num_tasks=1;
      mpi_task_id=0;
      mpi_ismaster=true;
      ismpi = false;
    #endif

    #if defined LIBJ_OMP
      #pragma omp parallel shared(omp_num_threads) 
      {
        #pragma omp single
        {
          omp_num_threads = omp_get_num_threads(); 
        }
      }
      isomp = true;
    #else
      omp_num_threads=1;
      isomp = false;
    #endif
    return 0;
  }
  
  //Destruction
  int destroy() 
  {
    #if defined LIBJ_MPI
      MPI_Finalize();
    #endif
    return 0;
  }

};

#endif
