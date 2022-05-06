#include "tensor.hpp"
#include "tensor_matrix.hpp"
#include "scatter_matrix.hpp"
#include "block_scatter_matrix.hpp"
#include <stdio.h>


void foo3(const libj::tensor_matrix<double>& B)
{
  printf("foo3\n");
  const libj::block_scatter_matrix<double> M(B,2,2);
  printf("Number of row blocks %zu\n",M.block_num(0));
  printf("Number of col blocks %zu\n",M.block_num(1));
  printf("Row block size %zu\n",M.block_size(0));
  printf("col block size %zu\n",M.block_size(1));

  printf("row blocks \n");
  for (size_t block=0;block<M.block_num(0);block++)
  {
    printf("Row block %zu has num %zu and stride %zu \n",block,M.block_size(0,block),
           M.block_stride(0,block));
  }
  printf("col blocks \n");
  for (size_t block=0;block<M.block_num(1);block++)
  {
    printf("Col block %zu has num %zu and stride %zu \n",block,M.block_size(1,block),
           M.block_stride(1,block));
  }
  
  for (size_t J=0;J<M.size(1);J++)
  {
    for (size_t I=0;I<M.size(0);I++)
    {
      printf("[%zu %zu] = %lf \n",I,J,M(I,J));
    }
  }

}

void foo2(const libj::tensor_matrix<double>& B)
{
  const libj::scatter_matrix<double> M = B;
  printf("foo2\n");
  for (size_t J=0;J<M.size(1);J++)
  {
    for (size_t I=0;I<M.size(0);I++)
    {
      printf("[%zu %zu] = %lf \n",I,J,M(I,J));
    }
  }

}

void foo(const libj::tensor<double>& A)
{
  libj::tensor_matrix<double> MAT(A,"ac","b");
  
  printf("bundle sizes of MAT %zu %zu \n",MAT.size(0),MAT.size(1));
  printf("Total size of MAT %zu \n",MAT.size());
  for (size_t J=0;J<MAT.size(1);J++)
  {
    for (size_t I=0;I<MAT.size(0);I++)
    {
      printf("[%zu %zu] = %lf \n",I,J,MAT(I,J));
    }
  } 

  foo2(MAT.block(0,0,MAT.size(0),2));
  foo2(MAT.block(0,2,MAT.size(0),1));
  
  foo2(MAT.block(0,0,3,MAT.size(1)));
  foo2(MAT.block(3,0,3,MAT.size(1)));

  foo3(MAT);
}

int main()
{
  libj::tensor<double> A(3,3,2);
  printf("boo boo\n");
  
  for (size_t I=0;I<A.size();I++)
  {
    A[I] = I;
  }

  libj::tensor_matrix<double> B(A,"","");
  
  foo(A);
}
