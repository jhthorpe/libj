#include <stdio.h>
#include "gpu_handler.hpp"

//Specific kernal to use
const char* hello = "\n"
"__kernel void hello(__global char* a)\n"
"{\n"
"  a[0] = 'H';\n"
"  a[1] = 'e';\n"
"  a[2] = 'l';\n"
"  a[3] = 'l';\n"
"  a[4] = 'o';\n"
"}\n";

int main()
{
  char AA[32];

  //This initializes the GPU handler and gpus
  libj::GPU_HANDLER GPU;
  GPU.print_gpu_info();

  //load the program
  GPU.load_program(hello); 

  //add buffer, by default this is blocking
  cl_int err;
  cl_mem buff = clCreateBuffer(GPU.platform.context,CL_MEM_READ_WRITE,32*sizeof(char),NULL,&err);
  if (err != CL_SUCCESS) {printf("create buffer errored \n");}

  //Set kernel and arguments
  //create a kernal with arguments  
  libj::GPU_KERNEL kernel;
  kernel.create(GPU.program,"hello");
  kernel.set_arg(0,sizeof(buff),(void *)&buff);

  //call the command on just one GPU
  size_t global_work_size = 32;
  GPU.gpus[0].queue_command(kernel,1,&global_work_size,NULL); 

  //read memory back
  GPU.gpus[0].read_to_host(buff,0,sizeof(char)*32,AA); 

  //return
  clFinish(GPU.gpus[0].commands);

  //test
  printf("%s \n",AA);

   
  return 0;
}
