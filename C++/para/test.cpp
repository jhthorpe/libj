#include "para.hpp"
#include <stdio.h>


int main()
{
  Para para;
  para.init();

  //filesystem "map"
  const int jobdata_file_id = para.file_add("jobdata");
  const int jobdata2_file_id = para.file_add("jobdata2");

  para.print_now("file id is %d \n",jobdata_file_id);
  para.print_now("file id 2 is %d \n",jobdata2_file_id);
  para.print_master_now("only master says this \n");


  para.file_open(jobdata_file_id,"wb");
  para.file_print_info();
  para.print_all();

  para.file_save();

  /*
  Pworld pworld;
  pworld.init();

  Pprint pprint;
  pprint.init(pworld);

  Pfile pfile;
  pfile.init(pworld);

  Pdata pdata;

  //store on file "jobdata"
  const long file_id = pfile.sadd(pworld,"jobdata");
  if (file_id == -1) {printf("error on task %d \n",pworld.mpi_world_task_id);
                      pworld.error(1);}
  
  const long list_id = pdata.add_list(100,file_id,sizeof(double));

  pdata.add_index(list_id,pworld.mpi_shared_root,file_id,100);
  pdata.add_index(list_id,pworld.mpi_shared_root,file_id,250);

  pdata.print_lists(pworld,pprint);
  pprint.print_all(pworld);
  pprint.reset(); 

  pprint.destroy(pworld);
  pworld.destroy();
  */
  para.destroy();
}
