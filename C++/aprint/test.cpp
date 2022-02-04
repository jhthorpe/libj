#include "aprint.hpp"

#include <mpi.h>


int main()
{
  Printbuffer buf;

  MPI_Init(NULL,NULL);
  int mpi_num_tasks;
  int mpi_task_id;

  MPI_Comm_size(MPI_COMM_WORLD,&mpi_num_tasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_task_id);

  const int num_message=2;
  buf.addstore("First thing from task number %d\n",mpi_task_id);
  buf.addstore("Second thing from task number %d\n",mpi_task_id);

  MPI_Barrier(MPI_COMM_WORLD);

  //Buffer for print buffers
  char* buffer;
  if (mpi_task_id == 0) {buffer = (char*) malloc(sizeof(char)*APRINT_LEN*mpi_num_tasks);}

  //For the number of messages
  for (int message=0;message<num_message;message++)
  {

    //Master task writes to itself
    if (mpi_task_id == 0) {std::strcpy(buffer,buf.vec[message]);}

    //Now, send and recieve (in order!!!)
    for (int task=1;task<mpi_num_tasks;task++)
    {

      //send
      if (task == mpi_task_id)
      {
        MPI_Send(buf.vec[message],APRINT_LEN,MPI_CHAR,0,42,MPI_COMM_WORLD);
      }

      //recieve
      if (mpi_task_id == 0)
      {
        MPI_Recv(buffer+APRINT_LEN*task,APRINT_LEN,MPI_CHAR,task,
                 42,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      }

      MPI_Barrier(MPI_COMM_WORLD); 

    }//loop over tasks

    //print data (in order!!!)
    if (mpi_task_id==0)
    {
      for (int task=0;task<mpi_num_tasks;task++) {printf("%s",buffer+APRINT_LEN*task);}    
    }
    MPI_Barrier(MPI_COMM_WORLD);

  }//end loop over messages


  MPI_Finalize();


  /*
  buf.addstore("1 2 3 4 = %d %4d %d %lf\n",1,2,3,(double)4);
  buf.add("\n\n");
  buf.add("the quick red fox jumped over the lazy brown dog");
  buf.add("doggy was not happy");
  buf.add("\nthis entry is getting very very very very long\n"); 
  buf.add("but that is kinda the point\n");
  buf.store();
  buf.print_all();
  */
}
