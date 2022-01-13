/*-------------------------------------------------------
  gemat.cpp
  .cpp file for gemat class, desinged to deal with 
  general matrices. See .hpp file for usage details 
-------------------------------------------------------*/
#include "gemat.hpp"

/*-------------------------------------------------------
  Constructors
-------------------------------------------------------*/
template<typename T>
gemat<T>::gemat() 
{
  m_buf = NULL;
  m_ptr = NULL;
  m_len = 0;
  m_nrow = 0;
  m_ncol = 0;
  m_alignment = 0;
  m_assigned = false;
  m_allocated = false;
}
template gemat<double>::gemat();
template gemat<float>::gemat();
template gemat<long>::gemat();
template gemat<int>::gemat();
template gemat<double*>::gemat();

template<typename T>
gemat<T>::gemat(const long n, const long m)
{
  m_allocated = false;
  m_assigned = false;
  allocate(n,m);
}
template gemat<double>::gemat(const long n, const long m);
template gemat<float>::gemat(const long n, const long m);
template gemat<long>::gemat(const long n, const long m);
template gemat<int>::gemat(const long n, const long m);
template gemat<double*>::gemat(const long n, const long m);

template <typename T>
gemat<T>::gemat(const long n, const long m, T* ptr)
{
  m_allocated = false;
  m_assigned = false;
  assign(n,m,ptr);
}
template gemat<double>::gemat(const long n, const long m, double* ptr);
template gemat<float>::gemat(const long n, const long m, float* ptr);
template gemat<long>::gemat(const long n, const long m, long* ptr);
template gemat<int>::gemat(const long n, const long m, int* ptr);
template gemat<double*>::gemat(const long n, const long m, double** ptr);

/*-------------------------------------------------------
  Destructors 
-------------------------------------------------------*/
template <typename T>
gemat<T>::~gemat()
{ 
  free();
}
template gemat<double>::~gemat();
template gemat<float>::~gemat();
template gemat<long>::~gemat();
template gemat<int>::~gemat();
template gemat<double*>::~gemat();
/*-------------------------------------------------------
 * calc_alignment()
 * calculates the alignment of m_buf
-------------------------------------------------------*/
template<typename T>
void gemat<T>::calc_alignment()
{
  m_alignment = 1;
  //2,4,8,16,32,64,128
  for (long m=2;m<=128;m*=2)
  {
    if ((long)m_buf%m != 0) {return;}
    m_alignment *= 2;
  }
  return;
}
template void gemat<double>::calc_alignment();
template void gemat<float>::calc_alignment();
template void gemat<long>::calc_alignment();
template void gemat<int>::calc_alignment();
template void gemat<double*>::calc_alignment();

/*-------------------------------------------------------
  aligned_allocate 
-------------------------------------------------------*/
template <typename T>
void gemat<T>::aligned_allocate(const int ALIGN, const long n, const long m)
{
  const long ll = n*m;
  const long mm=std::numeric_limits<long>::max(); //gives largest long 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll <= mm) 
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll <= mm) 
  {
    m_ptr = (T*) malloc(ALIGN+ll*sizeof(T));
//  } else if (ll < 1) {
//    printf("Attempted to allocate gemat of < 1 element \n");
  } else if (ll < 0) {
    printf("Attempted to allocate gemat of < 0 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to m_allocated gemat > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set gemat \n");
    exit(1);
  }

  if (m_ptr != NULL)
  {
    //align the buffer
    long M = (long)m_ptr%(long)ALIGN; //number of bytes off
    if (M != 0)
    {
      //shift m_buf start to match boundary
      char* cptr = (char*) m_ptr;
      for (long i=0;i<((long)ALIGN-M);i++)
      {
        ++cptr;
      }
      m_buf = (T*) cptr;
    } else {
      m_buf = m_ptr;
    }

    //set variables
    m_len = ll;
    m_nrow = n;
    m_ncol = m;
    m_allocated = true;
    m_assigned = false;
    calc_alignment();

  } else {
    printf("malloc failed for gemat of %ld,%ld elements \n",n,m);
    exit(1);
  }

}
template void gemat<double>::aligned_allocate(const int ALIGN, const long n, const long m);
template void gemat<float>::aligned_allocate(const int ALIGN, const long n, const long m);
template void gemat<long>::aligned_allocate(const int ALIGN, const long n, const long m);
template void gemat<int>::aligned_allocate(const int ALIGN, const long n, const long m);
template void gemat<double*>::aligned_allocate(const int ALIGN, const long n, const long m);

/*-------------------------------------------------------
  Allocate 
-------------------------------------------------------*/
template <typename T>
void gemat<T>::allocate(const long n, const long m)
{
  const long ll = n*m;
  const long mm=std::numeric_limits<long>::max(); //gives largest long 
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll <= mm) 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll <= mm) 
  {
    m_ptr = (T*) malloc(ll*sizeof(T));
//  } else if (ll < 1) {
//    printf("Attempted to allocate gemat of < 1 element \n");
  } else if (ll < 0) {
    printf("Attempted to allocate gemat of < 0 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to m_allocated gemat > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set gemat \n");
    exit(1);
  }

  if (m_ptr != NULL)
  {
    m_buf = m_ptr;
    m_len = ll;
    m_nrow = n;
    m_ncol = m;
    m_allocated = true; 
    m_assigned = false;
    calc_alignment();
  } else {
    printf("malloc failed for gemat of %ld,%ld elements \n",n,m);
    exit(1);
  }
}
template void gemat<double>::allocate(const long n, const long m);
template void gemat<float>::allocate(const long n, const long m);
template void gemat<long>::allocate(const long n, const long m);
template void gemat<int>::allocate(const long n, const long m);
template void gemat<double*>::allocate(const long n, const long m);

/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
template <typename T>
void gemat<T>::assign(const long n, const long m, T* pntr)
{
  const long ll=n*m;
  const long mm=std::numeric_limits<long>::max();
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll < mm && pntr != NULL) 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll < mm && pntr != NULL) 
  {
    m_buf = pntr;
    m_ptr = pntr;
    m_len = ll;
    m_nrow = n;
    m_ncol = m;
    m_assigned = true;
    m_allocated = false;
  } else if (m_allocated || m_assigned) {
    printf("Attempted to assign an already set gemat \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to assign gemat with m_len < 1 \n");
  } else if (ll < 0) {
    printf("Attempted to assign gemat with m_len < 0 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to assign gemat > long limit \n");
    exit(1);
  } else if (pntr == NULL) {
    printf("Attempted to assign gemat to NULL pointer \n");
    exit(1);
  } else {
    printf("Some assignment condition of gemat not met \n");
    exit(1);
  }
  calc_alignment();
}
template void gemat<double>::assign(const long n, const long m, double* pntr);
template void gemat<float>::assign(const long n, const long m, float* pntr);
template void gemat<long>::assign(const long n, const long m, long* pntr);
template void gemat<int>::assign(const long n, const long m, int* pntr);
template void gemat<double*>::assign(const long n, const long m, double** pntr);

/*-------------------------------------------------------
   ressign 
	- if the matrix is already asigned, reassign 
	  it to a new location in memory
-------------------------------------------------------*/
template <typename T>
void gemat<T>::reassign(const long n, const long m, T* pntr)
{
  const long ll=n*m;
  const long mm=std::numeric_limits<long>::max();
//  if (m_assigned && ll >= 1 && ll < mm && pntr != NULL) 
  if (m_assigned && ll >= 0 && ll < mm && pntr != NULL) 
  {
    m_buf = pntr;
    m_ptr = pntr;
    m_len = ll;
    m_nrow = n;
    m_ncol = m;
    m_assigned = true;
    m_allocated = false;
  } else if (!m_assigned) {
    printf("Attempted to reassign an unassigned gemat \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to reassign gemat with m_len < 1 \n");
  } else if (ll < 0) {
    printf("Attempted to reassign gemat with m_len < 0 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to reassign gemat > long limit \n");
    exit(1);
  } else if (pntr == NULL) {
    printf("Attempted to reassign gemat to NULL pointer \n");
    exit(1);
  } else {
    printf("Some reassignment condition of gemat not met \n");
    exit(1);
  }
  calc_alignment();
}
template void gemat<double>::reassign(const long n, const long m, double* pntr);
template void gemat<float>::reassign(const long n, const long m, float* pntr);
template void gemat<long>::reassign(const long n, const long m, long* pntr);
template void gemat<int>::reassign(const long n, const long m, int* pntr);
template void gemat<double*>::reassign(const long n, const long m, double** pntr);

/*-------------------------------------------------------
   Deallocate
-------------------------------------------------------*/
template <typename T>
void gemat<T>::deallocate()
{
  if (m_allocated)  
  { 
    m_buf = NULL;
    std::free(m_ptr); 
    m_len = 0;
    m_nrow = 0;
    m_ncol = 0;
    m_alignment = 0;
    m_allocated = false;
  } else {
    printf("Attempted to deallocate unset gemat \n");
    exit(1);
  }
}
template void gemat<double>::deallocate();
template void gemat<float>::deallocate();
template void gemat<long>::deallocate();
template void gemat<int>::deallocate();
template void gemat<double*>::deallocate();
/*-------------------------------------------------------
   unassign 
-------------------------------------------------------*/
template <typename T>
void gemat<T>::unassign()
{
  if (m_assigned) {
    m_len = 0;
    m_nrow = 0;
    m_ncol = 0;
    m_buf = NULL;
    m_ptr = NULL;
    m_assigned = false;
    m_alignment = 0;
  } else {
    printf("Attempted to unassign an unset gemat \n");
    exit(1);
  }
}
template void gemat<double>::unassign();
template void gemat<float>::unassign();
template void gemat<long>::unassign();
template void gemat<int>::unassign();
template void gemat<double*>::unassign();
/*-------------------------------------------------------
   free 
-------------------------------------------------------*/
template<typename T>
void gemat<T>::free()
{
  if (m_allocated) 
  {
    deallocate();
  } else if (m_assigned) {
    unassign();
  } else {
    m_buf = NULL;
    m_ptr = NULL;
    m_len = 0;
    m_nrow = 0;
    m_ncol = 0;
    m_alignment = 0;
    m_allocated = false;
    m_assigned = false;
  }
}
template void gemat<double>::free();
template void gemat<float>::free();
template void gemat<long>::free();
template void gemat<int>::free();
template void gemat<double*>::free();

/*-------------------------------------------------------
   info
    - prints informaiton about the vector
-------------------------------------------------------*/
template <typename T>
void gemat<T>::info() const
{
  if (m_assigned || m_allocated) 
  {
    printf("gemat has %ld elements \n",m_len);
    printf("gemat has %ld rows \n",m_nrow);
    printf("gemat has %ld cols \n",m_ncol);
    printf("gemat is aligned to %d bytes \n",m_alignment);
    printf("gemat points to  %p \n",(void *) m_ptr);
    printf("gemat buffer starts at  %p \n",(void *) m_buf);
  } else {
    printf("gemat is unset \n");
    printf("gemat has %ld elements \n",m_len);
    printf("gemat has %ld rows \n",m_nrow);
    printf("gemat has %ld cols \n",m_ncol);
  }
}
template void gemat<double>::info() const;
template void gemat<float>::info() const;
template void gemat<long>::info() const;
template void gemat<int>::info() const;
template void gemat<double*>::info() const;
/*-------------------------------------------------------
  zero()
	- zeros the matrix 
-------------------------------------------------------*/
template <typename T>
void gemat<T>::zero()
{
  assert(m_allocated||m_assigned);
  const long ll=m_len-1;
  long i=0;
  while (true)
  {
    *(m_buf+i) = 0;
    if (i >= ll) break;
    i++;
  }
}
template void gemat<double>::zero();
template void gemat<float>::zero();
template void gemat<long>::zero();
template void gemat<int>::zero();
/*-------------------------------------------------------
  I() 
	- makes the main diagonal of the matrix = 1 
        - one could do this in two setps, but 
            here we only make one pass through the 
            m_buffer

   example
   1 0 0      1 0 0 0      1 0 0
   0 1 0  or  0 1 0 0  or  0 1 0
   0 0 1      0 0 1 0      0 0 1
   0 0 0

-------------------------------------------------------*/
template <typename T>
void gemat<T>::I()
{
  assert(m_allocated||m_assigned);

  //find number of iterations to traverse
  const long nitr=(m_nrow <= m_ncol) ? m_nrow : m_ncol; 
  const long nr=m_nrow+1;
  const long l1=m_len-1;
  const long l2=nitr-1;

  long i=0;
  long itr=0;
  while(true)
  {
    *(m_buf+i) = (T) 1; //diagonal element

    //fast forward to next diagonal
    if (itr >= l2) break;
    i++;
    itr++;
    
    //here we want the extra i++
    //fill in zeros up to next diagonal
    while(i<nr*itr)
    {
      *(m_buf+i) = (T) 0;
      i++;
    }
  }

  //cleanup at end
  //i starts on final diagonal
  while(true)
  {
    if (i>=l1) break;
    i++;
    *(m_buf+i) = (T) 0;
  }

}
template void gemat<double>::I();
template void gemat<float>::I();
template void gemat<long>::I();
template void gemat<int>::I();
/*-------------------------------------------------------
   =
        operator overloading to set matrix equal to constant
-------------------------------------------------------*/
template <typename T>
void gemat<T>::operator= (const T a)
{
  assert (m_allocated||m_assigned);
  long i=0;
  const long ll=m_len-1;
  while (true)
  {
    *(m_buf+i) = a;
    if (i >= ll) break;
    i++;
  }
}
template void gemat<double>::operator= (const double a);
template void gemat<float>::operator= (const float a);
template void gemat<long>::operator= (const long a);
template void gemat<int>::operator= (const int a);
/*-------------------------------------------------------
  print
	- prints elements of vector
-------------------------------------------------------*/
template <>
void gemat<double>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<m_nrow;i++)
    {
      printf("[%ld,%ld]    %18.15E \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}

template <>
void gemat<float>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<m_nrow;i++)
    {
      printf("[%ld,%ld]    %10.7E \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}

template <>
void gemat<long>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<m_nrow;i++)
    {
      printf("[%ld,%ld]    %ld \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}

template <>
void gemat<int>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<m_nrow;i++)
    {
      printf("[%ld,%ld]    %d \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}

template <>
void gemat<double*>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<m_nrow;i++)
    {
      printf("[%ld,%ld]    %p \n",i,j,(void*) *(m_buf+xx));
      xx++;
    }
  }
}
