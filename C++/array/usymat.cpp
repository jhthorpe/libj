/*-------------------------------------------------------
  usymat.cpp
  .cpp file for usymat class, desinged to deal with 
  upper symmetric matricies 

  usaged described in usymat.hpp
-------------------------------------------------------*/
#include "usymat.hpp"

/*-------------------------------------------------------
  Constructors
-------------------------------------------------------*/
template <typename T>
usymat<T>::usymat() 
{
  m_buf = NULL;
  m_ptr = NULL;
  m_len = 0;
  m_ncol = 0;
  m_alignment = 0;
  m_assigned = false;
  m_allocated = false;
}
template usymat<double>::usymat();
template usymat<float>::usymat();
template usymat<long>::usymat();
template usymat<int>::usymat();

template <typename T>
usymat<T>::usymat(const long n, const long m)
{
  m_allocated = false;
  m_assigned = false;
  allocate(n,m);
}
template usymat<double>::usymat(const long n, const long m);
template usymat<float>::usymat(const long n, const long m);
template usymat<long>::usymat(const long n, const long m);
template usymat<int>::usymat(const long n, const long m);

template <typename T>
usymat<T>::usymat(const long n, const long m, T* ptr)
{
  m_allocated = false;
  m_assigned = false;
  assign(n,m,ptr);
}
template usymat<double>::usymat(const long n, const long m, double* ptr);
template usymat<float>::usymat(const long n, const long m, float* ptr);
template usymat<long>::usymat(const long n, const long m, long* ptr);
template usymat<int>::usymat(const long n, const long m, int* ptr);

/*-------------------------------------------------------
  Destructors 
-------------------------------------------------------*/
template <typename T>
usymat<T>::~usymat()
{ 
  if (m_assigned)
  { 
    m_buf = NULL;
    m_ptr = NULL; 
  } else if (m_allocated) {
    std::free(m_ptr);
    m_buf = NULL; 
  }
}
template usymat<double>::~usymat();
template usymat<float>::~usymat();
template usymat<long>::~usymat();
template usymat<int>::~usymat();

/*-------------------------------------------------------
 * calc_alignment()
 * calculates the alignment of m_buf
-------------------------------------------------------*/
template<typename T>
void usymat<T>::calc_alignment()
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
template void usymat<double>::calc_alignment();
template void usymat<float>::calc_alignment();
template void usymat<long>::calc_alignment();
template void usymat<int>::calc_alignment();

/*-------------------------------------------------------
  aligned_allocate 
-------------------------------------------------------*/
template <typename T>
void usymat<T>::aligned_allocate(const int ALIGN, const long n, const long m)
{
  const long ll = m*(m+1)/2;
  const long mm=std::numeric_limits<long>::max(); //gives largest long 
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll <= mm && n == m) 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll <= mm && n == m) 
  {
    m_ptr = (T*) malloc(ALIGN+ll*sizeof(T));
  } else if (n != m) {
    printf("Attempted to allocate usymat where nrow != m_ncol \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to allocate usymat of < 1 element \n");
  } else if (ll < 0) {
    printf("Attempted to allocate usymat of < 0 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to m_allocated usymat > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set usymat \n");
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

    m_buf = m_ptr;
    m_len = ll;
    m_ncol = m;
    m_allocated = true; 
    m_assigned = false;
    calc_alignment();
  } else {
    printf("malloc failed of usymat of %ld,%ld elements \n",n,m);
    exit(1);
  }
}
template void usymat<double>::aligned_allocate(const int ALIGN, const long n, const long m);
template void usymat<float>::aligned_allocate(const int ALIGN, const long n, const long m);
template void usymat<long>::aligned_allocate(const int ALIGN, const long n, const long m);
template void usymat<int>::aligned_allocate(const int ALIGN, const long n, const long m);
/*-------------------------------------------------------
  Allocate 
-------------------------------------------------------*/
template <typename T>
void usymat<T>::allocate(const long n, const long m)
{
  const long ll = m*(m+1)/2;
  const long mm=std::numeric_limits<long>::max(); //gives largest long 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll <= mm && n == m) 
  {
    m_ptr = (T*) malloc(ll*sizeof(T));
  } else if (n != m) {
    printf("Attempted to allocate usymat where nrow != m_ncol \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to allocate usymat of < 1 element \n");
  } else if (ll < 0) {
    printf("Attempted to allocate usymat of < 0 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to m_allocated usymat > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set usymat \n");
    exit(1);
  }

  if (m_ptr != NULL)
  {
    m_buf = m_ptr;
    m_len = ll;
    m_ncol = m;
    m_allocated = true; 
    m_assigned = false;
    calc_alignment();
  } else {
    printf("malloc failed of usymat of %ld,%ld elements \n",n,m);
    exit(1);
  }
}
template void usymat<double>::allocate(const long n, const long m);
template void usymat<float>::allocate(const long n, const long m);
template void usymat<long>::allocate(const long n, const long m);
template void usymat<int>::allocate(const long n, const long m);

/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
template <typename T>
void usymat<T>::assign(const long n, const long m, T* ptr)
{
  const long ll=m*(m+1)/2;
  const long mm=std::numeric_limits<long>::max();
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll < mm 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll < mm 
      && ptr != NULL && n==m) 
  {
    m_ptr = ptr;
    m_buf = ptr;
    m_len = ll;
    m_ncol = n;
    m_assigned = true;
    m_allocated = false;
    calc_alignment();
  } else if (n != m) {
    printf("Attempted to assign usymat where nrow != m_ncol \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to assign an already set usymat \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to assign usymat with m_len < 1 \n");
  } else if (ll < 0) {
    printf("Attempted to assign usymat with m_len < 0 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to assign usymat > long limit \n");
    exit(1);
  } else if (ptr == NULL) {
    printf("Attempted to assign usymat to NULL pointer \n");
    exit(1);
  } else {
    printf("Some assignment condition of usymat not met \n");
    exit(1);
  }
}
template void usymat<double>::assign(const long n, const long m, double* ptr);
template void usymat<float>::assign(const long n, const long m, float* ptr);
template void usymat<long>::assign(const long n, const long m, long* ptr);
template void usymat<int>::assign(const long n, const long m, int* ptr);
/*-------------------------------------------------------
   ressign 
	-if usymat is already assigned, reassign to 
	 new location
-------------------------------------------------------*/
template <typename T>
void usymat<T>::reassign(const long n, const long m, T* ptr)
{
  const long ll=m*(m+1)/2;
  const long mm=std::numeric_limits<long>::max();
//  if (m_assigned && ll >= 1 && ll < mm 
  if (m_assigned && ll >= 0 && ll < mm 
      && ptr != NULL && n==m) 
  {
    m_ptr = ptr;
    m_buf = ptr;
    m_len = ll;
    m_ncol = n;
    m_assigned = true;
    m_allocated = false;
    calc_alignment();
  } else if (n != m) {
    printf("Attempted to reassign usymat where nrow != m_ncol \n");
    exit(1);
  } else if (!m_assigned) {
    printf("Attempted to reassign an unassigned usymat \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to reassign usymat with m_len < 1 \n");
  } else if (ll < 0) {
    printf("Attempted to reassign usymat with m_len < 0 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to reassign usymat > long limit \n");
    exit(1);
  } else if (ptr == NULL) {
    printf("Attempted to reassign usymat to NULL pointer \n");
    exit(1);
  } else {
    printf("Some reassignment condition of usymat not met \n");
    exit(1);
  }
}
template void usymat<double>::reassign(const long n, const long m, double* ptr);
template void usymat<float>::reassign(const long n, const long m, float* ptr);
template void usymat<long>::reassign(const long n, const long m, long* ptr);
template void usymat<int>::reassign(const long n, const long m, int* ptr);

/*-------------------------------------------------------
   Deallocate
-------------------------------------------------------*/
template <typename T>
void usymat<T>::deallocate()
{
  if (m_allocated)  
  { 
    m_buf = NULL;
    std::free(m_ptr); 
    m_len = 0;
    m_ncol = 0;
    m_allocated = false;
    m_alignment = 0;
  } else {
    printf("Attempted to deallocate unset usymat \n");
    exit(1);
  }
}
template void usymat<double>::deallocate();
template void usymat<float>::deallocate();
template void usymat<long>::deallocate();
template void usymat<int>::deallocate();
/*-------------------------------------------------------
   unassign 
-------------------------------------------------------*/
template <typename T>
void usymat<T>::unassign()
{
  if (m_assigned) {
    m_buf = NULL;
    m_ptr = NULL;
    m_len = 0;
    m_ncol = 0;
    m_alignment = 0;
    m_assigned = false;
  } else {
    printf("Attempted to unassign an unset usymat \n");
    exit(1);
  }
}
template void usymat<double>::unassign();
template void usymat<float>::unassign();
template void usymat<long>::unassign();
template void usymat<int>::unassign();
/*-------------------------------------------------------
  free 
-------------------------------------------------------*/
template <typename T>
void usymat<T>::free()
{
  if (m_assigned)
  {
    unassign(); 
  } else if (m_allocated) {
    deallocate(); 
  } else {
    m_buf = NULL;
    m_ptr = NULL;
    m_len = 0;
    m_ncol = 0;
    m_alignment = 0;
    m_assigned = false; 
    m_allocated = false;
  }
}
template void usymat<double>::free();
template void usymat<float>::free();
template void usymat<long>::free();
template void usymat<int>::free();
/*-------------------------------------------------------
   info
    - prints informaiton about the vector
-------------------------------------------------------*/
template <typename T>
void usymat<T>::info() const
{
  if (m_assigned || m_allocated) 
  {
    printf("usymat has %ld elements \n",m_len);
    printf("usymat has %ld cols \n",m_ncol);
    printf("usymat points to  %p \n",(void *) m_ptr);
    printf("usymat buffer begins at %p \n",(void*) m_buf);
    printf("usymat is aligned to %d bytes \n",m_alignment);
  } else {
    printf("usymat is unset \n");
    printf("usymat has %ld elements \n",m_len);
    printf("usymat has %ld cols \n",m_ncol);
  }
}
template void usymat<double>::info() const;
template void usymat<float>::info() const;
template void usymat<long>::info() const;
template void usymat<int>::info() const;
/*-------------------------------------------------------
  zero()
	- zeros the matrix 
-------------------------------------------------------*/
template <typename T>
void usymat<T>::zero()
{
  assert(m_allocated||m_assigned);
  const long ll=m_len-1;
  long i=0;
  while (true)
  {
    *(m_buf+i) = (T) 0;
    if (i >= ll) break;
    i++;
  }
}
template void usymat<double>::zero();
template void usymat<float>::zero();
template void usymat<long>::zero();
template void usymat<int>::zero();
/*-------------------------------------------------------
  I() 
	- makes the main diagonal of the matrix = 1 
        - one could do this in two setps, but 
            here we only make one pass through the 
            m_buffer

-------------------------------------------------------*/
template <typename T>
void usymat<T>::I()
{
  assert(m_allocated||m_assigned);

  //find number of iterations to traverse
  const long nitr=m_ncol-1;
  long tmp;
  long i=0;
  long col=0;
  while(true)
  {
    *(m_buf+i) = (T) 1; //diagonal element
    
    //zero up to next diagonal
    if (col >= nitr) break;
    col++;
    
    tmp = i+col;
    while(i<=tmp)
    {
      i++;
      *(m_buf+i) = (T) 0;
    }
    
  }

}
template void usymat<double>::I();
template void usymat<float>::I();
template void usymat<long>::I();
template void usymat<int>::I();
/*-------------------------------------------------------
   =
        operator overloading to set matrix equal to constant
-------------------------------------------------------*/
template <typename T>
void usymat<T>::operator= (const T a)
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
template void usymat<double>::operator= (const double a);
template void usymat<float>::operator= (const float a);
template void usymat<long>::operator= (const long a);
template void usymat<int>::operator= (const int a);
/*-------------------------------------------------------
  print
	- prints elements of vector
-------------------------------------------------------*/
template <>
void usymat<double>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<=j;i++)
    {
      printf("[%ld,%ld]    %18.15E \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}

template <>
void usymat<float>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<=j;i++)
    {
      printf("[%ld,%ld]    %10.7E \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}


template <>
void usymat<long>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<=j;i++)
    {
      printf("[%ld,%ld]    %ld \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}

template <>
void usymat<int>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long j=0;j<m_ncol;j++)
  {
    for (long i=0;i<=j;i++)
    {
      printf("[%ld,%ld]    %d \n",i,j,*(m_buf+xx));
      xx++;
    }
  }
}
