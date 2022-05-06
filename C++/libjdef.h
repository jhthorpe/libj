//PARALLELIZATION DEFINITIONS
#define LIBJ_MPI 1
#if defined (_OPENMP)
  #define LIBJ_OMP 1
#endif

//CACHE SIZE DEFINITIONS
#define L1_BYTES 32768
#define L2_BYTES 262144
#define LINE_BYTES 64

//ALIGNMENT DEFINITIONS
#define DOUBLE_ALIGN 32
#define FLOAT_ALIGN 32
#define LONG_ALIGN 32
#define INT_ALIGN 32
#define MAX_ALIGN DOUBLE_ALIGN

//compiler specific definitions
#define LIBJ_RESTRICT __restrict__
