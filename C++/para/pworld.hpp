/*-----------------------------------------------------------------
  pworld.hpp
	JHT, Febuary 7, 2022 : created

  .hpp file for Pworld, which manages the initialization and 
  finalization of MPI parameters if they are required. This struct
  also carries most of the general MPI information

//Usage
init()		: initializes variables and structures
destroy()	: finalizes variables and structures

//Communicators
comm_world 	: MPI_COMM_WORLD
comm_shared	: MPI_COMM_TYPE_SHARED

-----------------------------------------------------------------*/
#ifndef LIBJ_PWORLD_HPP
#define LIBJ_PWORLD_HPP
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
  int status;			//status
  int mpi_world_num_tasks;	//world task size
  int mpi_world_task_id;	//world task id
  int mpi_shared_num_tasks;	//shared task size
  int mpi_shared_task_id;	//shared task id
  int mpi_shared_root;		//shared root id
  int omp_num_threads;		//number of OMP threads
  bool ismpi;			//has mpi
  bool isomp;			//has omp
  bool mpi_world_ismaster;	//is world master 
  bool mpi_shared_ismaster;	//is shared master 
  bool mpi_doesIO;		//if true, this task does file IO

  //MPI communicators
  #if defined LIBJ_MPI
    MPI_Comm comm_world; 	//world communicator
    MPI_Comm comm_shared;	//shared communicator
    MPI_Info mpi_info;		//info
  #endif

  //Initialize
  int init();
  
  //Destruction
  int destroy();

};

#endif
