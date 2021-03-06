/*----------------------------------------------------------------------
  zero.hpp
	JHT, April 11, 2022 : created
	JHT, May 18, 2022   : modified to header only

  .cpp file for the zero function, which sets a tensor to zero. This
  is coded to work best on larger tensors

  General flow is as follows

  1) convert tensor to col-vector tensor_matrix. Zero out a "packed"
     vector
   
  2) parallel loop through pannels of vector, sized to fit in L2
     cache (which is assumed not to be shared)

  3) loop through packs of L1 size and   

----------------------------------------------------------------------*/
#include <stdio.h>
#include "jblis_level1.hpp"
#include "block_scatter_matrix2.hpp"
#include "tensor_matrix2.hpp"
#include "tensor.hpp"

//Note that the block size is the same as the microkernel size, here
#define ROW_BLOCK_SIZE 16
#define COL_BLOCK_SIZE 1

namespace libj
{

/*----------------------------------------------------------------------
  zero_microkernel_stride1
	special code for if the row stride of the current block is zero
----------------------------------------------------------------------*/
template <typename T>
inline void zero_microkernel_stride1(T* A)
{
  A[0] = (T) 0;   
  A[1] = (T) 0;   
  A[2] = (T) 0;   
  A[3] = (T) 0;   
  A[4] = (T) 0;   
  A[5] = (T) 0;   
  A[6] = (T) 0;   
  A[7] = (T) 0;   

  A[8] = (T) 0;   
  A[9] = (T) 0;   
  A[10] = (T) 0;   
  A[11] = (T) 0;   
  A[12] = (T) 0;   
  A[13] = (T) 0;   
  A[14] = (T) 0;   
  A[15] = (T) 0;   

}

/*----------------------------------------------------------------------
  zero_microkernel_stride1
	special code for doubles with stride 1
----------------------------------------------------------------------*/
#if defined LIBJ_AVX
template<>
void inline zero_microkernel_stride1(double* A)
{
  const __m256d a_0_3   = _mm256_setzero_pd(); 
  const __m256d a_4_7   = _mm256_setzero_pd(); 
  const __m256d a_8_11  = _mm256_setzero_pd(); 
  const __m256d a_12_15 = _mm256_setzero_pd(); 
  _mm256_storeu_pd(A+0,a_0_3);
  _mm256_storeu_pd(A+4,a_4_7);
  _mm256_storeu_pd(A+8,a_8_11);
  _mm256_storeu_pd(A+12,a_12_15);
}
#endif
/*----------------------------------------------------------------------
  zero_microkernel_strideg
	special code for constant stride blocks 	
----------------------------------------------------------------------*/
template <typename T>
inline void zero_microkernel_strideg(T* A, const size_t stride)
{
  A[0*stride] = (T) 0;   
  A[1*stride] = (T) 0;   
  A[2*stride] = (T) 0;   
  A[3*stride] = (T) 0;   
  A[4*stride] = (T) 0;   
  A[5*stride] = (T) 0;   
  A[6*stride] = (T) 0;   
  A[7*stride] = (T) 0;   
  A[8*stride] = (T) 0;   
  A[9*stride] = (T) 0;   
  A[10*stride] = (T) 0;   
  A[11*stride] = (T) 0;   
  A[12*stride] = (T) 0;   
  A[13*stride] = (T) 0;   
  A[14*stride] = (T) 0;   
  A[15*stride] = (T) 0;   
}

/*----------------------------------------------------------------------
  zero_inner_kernel 
	takes in block scatter matrix, loops through cols and row
        blocks, sets the values to zero via microkernel calls 
----------------------------------------------------------------------*/
template <typename T,size_t ROWS, size_t COLS>
inline void zero_inner_kernel(const size_t START, const size_t LEN, 
  libj::block_scatter_matrix2<T,ROWS,COLS,ROW_BLOCK_SIZE,COL_BLOCK_SIZE>& BLOCK) 
{
  const size_t block_size = BLOCK.block_size(0);

  //cleanup the first row block
  size_t row = START;
  for (; row < std::min(BLOCK.next_block_index(0,START),START+LEN-1); row++)
  {
    BLOCK(row,0) = (T) 0;
  } 

  //general row blocks in between
  const size_t start_row_block = BLOCK.block_id(0,START);
  const size_t   end_row_block = BLOCK.block_id(0,START+LEN);
  for (size_t row_block = start_row_block+1; row_block < end_row_block; row_block++)
  {
    const size_t  block_stride = BLOCK.block_stride(0,row_block);
    if (block_stride == 1)     {zero_microkernel_stride1<T>(&BLOCK(row,0));} 
    else if (block_stride > 0) {zero_microkernel_strideg<T>(&BLOCK(row,0),
                                                            block_stride);} 
    else {
      for (size_t sub_row = row; sub_row < row + BLOCK.block_size(0); sub_row++) {
        BLOCK(sub_row,0) = (T) 0;
      }
    }                     
    row += block_size;  
  } 

  //cleanup the last row block 
  for (; row < START+LEN ; row++)
  {
    BLOCK(row,0) = (T) 0;
  } 
  
}

/*----------------------------------------------------------------------
  General code
----------------------------------------------------------------------*/
template <typename T, size_t NLHS, size_t NRHS>
void zero2(libj::tensor<T>& A)
{
  //set column vector tensor matrix
  libj::tensor_matrix2<T,NLHS,NRHS> A_MATRIX(A,"","");

  //Get parameters
  constexpr size_t PANEL_SIZE = libj::Cache::L2_elements<T>();
//  const size_t PACK_SIZE  = cache.L1_elements<T>(); 
  constexpr size_t PACK_SIZE  = 8*libj::Cache::LINE_elements<T>(); 
//  const size_t PACK_SIZE  = cache.L2_elements<T>()/2;
  constexpr size_t LINE_SIZE  = libj::Cache::LINE_elements<T>(); 

  //Initialize the block scatter matrix
  libj::block_scatter_matrix2<T,PACK_SIZE,1,ROW_BLOCK_SIZE,COL_BLOCK_SIZE> A_BLOCKED;

  //Loops
  const size_t start = 0;
  const size_t end   = A_MATRIX.size(); 
  #pragma omp parallel for shared(A_MATRIX) private(A_BLOCKED) schedule(static) 
  for (size_t panel_start = start; panel_start < end; panel_start += PANEL_SIZE)
  {
    const size_t panel_len = std::min(end-panel_start,PANEL_SIZE);
    const size_t panel_end = panel_start + panel_len;
    
    //loop over L1 packs of BA
    for (size_t pack_start = panel_start; pack_start < panel_end; 
         pack_start += PACK_SIZE)
    {
      const size_t pack_len = std::min(panel_end-pack_start,PACK_SIZE);
     
      //construct blocked scatter matrix for this pannel
//      A_BLOCKED.assign(A_MATRIX.block(pack_start,0,
//                                      pack_len,1),
//                       ROW_BLOCK_SIZE,COL_BLOCK_SIZE);
      A_BLOCKED.assign_to_block(A_MATRIX,pack_start,0);

      //call inner kernel on pannel
      zero_inner_kernel<T,PACK_SIZE,1>(0,pack_len,A_BLOCKED);

    }
  }
}

}//end of namespace
