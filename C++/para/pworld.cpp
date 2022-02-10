/*-----------------------------------------------------------------
  pworld.cpp
	JHT, Febuary 9, 2022 : created

  .cpp file for pworld
-----------------------------------------------------------------*/
#include "para.hpp"

//-----------------------------------------------------------------
// initialize
//-----------------------------------------------------------------
int Pworld::init()
{
  #if defined LIBJ_MPI
    ismpi = true;

    //initial setup
    MPI_Init(NULL,NULL);
    MPI_Info_create(&mpi_info);

    //MPI world setup
    comm_world = MPI_COMM_WORLD;
    MPI_Comm_size(comm_world,&mpi_world_num_tasks);
    MPI_Comm_rank(comm_world,&mpi_world_task_id);
    mpi_world_ismaster = (mpi_world_task_id != 0) ? false : true; 

    //MPI shared setup
    MPI_Comm_split_type(comm_world,MPI_COMM_TYPE_SHARED,
                        0,mpi_info,&comm_shared);
    MPI_Comm_size(comm_shared,&mpi_shared_num_tasks);
    MPI_Comm_rank(comm_shared,&mpi_shared_task_id);
    mpi_shared_ismaster = (mpi_shared_task_id != 0) ? false : true; 
    

  #else
    ismpi = false;

    mpi_world_num_tasks=1;
    mpi_world_task_id=0;
    mpi_world_ismaster=true;

    mpi_shared_num_tasks=1;
    mpi_shared_task_id=0;
    mpi_shared_ismaster=true;
  #endif

  #if defined LIBJ_OMP
    isomp = true;
    #pragma omp parallel 
    {
      #pragma omp single
      {
        omp_num_threads = omp_get_num_threads();
      }
    }
  #else
    isomp = false;
    omp_num_threads=1;
  #endif
  return 0;
}


//-----------------------------------------------------------------
// Destroy
//-----------------------------------------------------------------
int Pworld::destroy()
{
  #if defined LIBJ_MPI
    MPI_Finalize(); 
  #endif
  return 0;
}
