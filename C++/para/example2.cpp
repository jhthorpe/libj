#include "para.hpp"
#include "libjdef.h"

int main()
{
  Pworld pworld; //general info
  pworld.init();

  Pprint pprint; //print buffers
  pprint.init(pworld);

  pprint.addstore("hello from task %d of %d\n",pworld.mpi_task_id,pworld.mpi_num_tasks);
  pprint.addstore("bigggggg oof task %d of %d\n",pworld.mpi_task_id,pworld.mpi_num_tasks);
  pprint.print_all(pworld);

  pprint.print(pworld,0);

  pprint.destroy(pworld); //destroy the print buffers
  pworld.destroy(); //this must always be called last
  return 0;
}

