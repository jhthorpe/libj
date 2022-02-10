#include <stdio.h>
#include "para.hpp"
#include <mpi.h>

int main()
{
  int stat;
  Pworld pworld; //general info
  pworld.init();

  if (pworld.mpi_shared_ismaster)
  {
    printf("master of shared sees %d tasks\n",pworld.mpi_shared_num_tasks);  
  }

  //Pprint always sees the WORLD
  Pprint pprint; //print buffers
  pprint.init(pworld);

  Pfile  pfile;
  pfile.init(pworld);

  const int fid = pfile.sadd(pworld,"ff");
  stat = pfile.open(pworld,fid,"w+b");

  pfile.info(pworld,pprint);
  pprint.print_all(pworld);
  pprint.reset();

  if (pworld.mpi_doesIO)
  {
    int arr[10];
    for (int i=0;i<10;i++)
    {
      arr[i] = pworld.mpi_world_task_id + i;
    }
    pfile.write(fid,0,arr,sizeof(int),10);

    for (int i=0;i<10;i++)
    {
      pfile.read(fid,0,arr,sizeof(int),10); 
    }
  
    pprint.add("task #%d arr has : ",pworld.mpi_world_task_id);
    for (int i=0;i<10;i++)
    {
      pprint.add("%d ",arr[i]);
    }
  }
  pprint.addstore("\n");
  pprint.print_all(pworld);
  pprint.reset();

  //close all files and save
  pfile.save(pworld);

  pprint.destroy(pworld); //destroy the print buffers
  pworld.destroy(); //this must always be called last
  return 0;
}

