/*-------------------------------------------------------
  geten4.cpp
  .cpp file for geten4 class, desinged to deal with 
  general, 3 dimension tensors 
-------------------------------------------------------*/
#include "geten4.hpp"

/*-------------------------------------------------------
  Constructors
-------------------------------------------------------*/
template<typename T>
geten4<T>::geten4() 
{
  m_buf = NULL;
  m_ptr = NULL;
  m_len = 0;
  m_nd1 = 0;
  m_nd2 = 0;
  m_nd3 = 0;
  m_nd4 = 0;
  m_alignment = 0;
  m_assigned = false;
  m_allocated = false;
}
template geten4<double>::geten4();
template geten4<float>::geten4();
template geten4<long>::geten4();
template geten4<int>::geten4();

template<typename T>
geten4<T>::geten4(const long n, const long m, const long l, const long k)
{
  m_allocated = false;
  m_assigned = false;
  allocate(n,m,l,k);
}
template geten4<double>::geten4(const long n, const long m, const long l, const long k);
template geten4<float>::geten4(const long n, const long m, const long l, const long k);
template geten4<long>::geten4(const long n, const long m, const long l, const long k);
template geten4<int>::geten4(const long n, const long m, const long l, const long k);

template <typename T>
geten4<T>::geten4(const long n, const long m, const long l, const long k, T* ptr)
{
  m_allocated = false;
  m_assigned = false;
  assign(n,m,l,k,ptr);
}
template geten4<double>::geten4(const long n, const long m, const long l, const long k, double* ptr);
template geten4<float>::geten4(const long n, const long m, const long l, const long k, float* ptr);
template geten4<long>::geten4(const long n, const long m, const long l, const long k, long* ptr);
template geten4<int>::geten4(const long n, const long m, const long l, const long k, int* ptr);

/*-------------------------------------------------------
  Destructors 
-------------------------------------------------------*/
template <typename T>
geten4<T>::~geten4()
{ 
  free();
}
template geten4<double>::~geten4();
template geten4<float>::~geten4();
template geten4<long>::~geten4();
template geten4<int>::~geten4();

/*-------------------------------------------------------
 * calc_alignment()
 * calculates the alignment of m_buf
-------------------------------------------------------*/
template<typename T>
void geten4<T>::calc_alignment()
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
template void geten4<double>::calc_alignment();
template void geten4<float>::calc_alignment();
template void geten4<long>::calc_alignment();
template void geten4<int>::calc_alignment();

/*-------------------------------------------------------
  aligned_allocate 
-------------------------------------------------------*/
template <typename T>
void geten4<T>::aligned_allocate(const int ALIGN, const long n, const long m, const long l, const long k)
{
  const long ll = n*m*l*k;
  const long mm=std::numeric_limits<long>::max(); //gives largest long 
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll <= mm) 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll <= mm) 
  {
    m_ptr = (T*) malloc(ALIGN+ll*sizeof(T));
//  } else if (ll < 1) {
//    printf("Attempted to allocate geten4 of < 1 element \n");
  } else if (ll < 0) {
    printf("Attempted to allocate geten4 of < 0 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to m_allocated geten4 > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set geten4 \n");
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
    m_nd1 = n;
    m_nd2 = m;
    m_nd3 = l;
    m_nd4 = k;
    m_allocated = true;
    m_assigned = false;
    calc_alignment();

  } else {
    printf("malloc failed for geten4 of %ld,%ld,%ld,%ld elements \n",n,m,l,k);
    exit(1);
  }

}
template void geten4<double>::aligned_allocate(const int ALIGN, const long n, const long m, const long l, const long k);
template void geten4<float>::aligned_allocate(const int ALIGN, const long n, const long m, const long l, const long k);
template void geten4<long>::aligned_allocate(const int ALIGN, const long n, const long m, const long l, const long k);
template void geten4<int>::aligned_allocate(const int ALIGN, const long n, const long m, const long l, const long k);

/*-------------------------------------------------------
  Allocate 
-------------------------------------------------------*/
template <typename T>
void geten4<T>::allocate(const long n, const long m, const long l, const long k)
{
  const long ll = n*m*l*k;
  const long mm=std::numeric_limits<long>::max(); //gives largest long 
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll <= mm) 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll <= mm) 
  {
    m_ptr = (T*) malloc(ll*sizeof(T));
//  } else if (ll < 1) {
//    printf("Attempted to allocate geten4 of < 1 element \n");
  } else if (ll < 0) {
    printf("Attempted to allocate geten4 of < 0 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to m_allocated geten4 > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set geten4 \n");
    exit(1);
  }

  if (m_ptr != NULL)
  {
    m_buf = m_ptr;
    m_len = ll;
    m_nd1 = n;
    m_nd2 = m;
    m_nd3 = l;
    m_nd4 = k;
    m_allocated = true; 
    m_assigned = false;
    calc_alignment();
  } else {
    printf("malloc failed for geten4 of %ld,%ld,%ld,%ld elements \n",n,m,l,k);
    exit(1);
  }
}
template void geten4<double>::allocate(const long n, const long m, const long l, const long k);
template void geten4<float>::allocate(const long n, const long m, const long l, const long k);
template void geten4<long>::allocate(const long n, const long m, const long l, const long k);
template void geten4<int>::allocate(const long n, const long m, const long l, const long k);

/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
template <typename T>
void geten4<T>::assign(const long n, const long m, const long l, const long k, T* pntr)
{
  const long ll=n*m*l*k;
  const long mm=std::numeric_limits<long>::max();
//  if (!(m_allocated || m_assigned) && ll >= 1 && ll < mm && pntr != NULL) 
  if (!(m_allocated || m_assigned) && ll >= 0 && ll < mm && pntr != NULL) 
  {
    m_buf = pntr;
    m_ptr = pntr;
    m_len = ll;
    m_nd1 = n;
    m_nd2 = m;
    m_nd3 = l;
    m_nd4 = k;
    m_assigned = true;
    m_allocated = false;
  } else if (m_allocated || m_assigned) {
    printf("Attempted to assign an already set geten4 \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to assign geten4 with m_len < 1 \n");
  } else if (ll < 0) {
    printf("Attempted to assign geten4 with m_len < 0 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to assign geten4 > long limit \n");
    exit(1);
  } else if (pntr == NULL) {
    printf("Attempted to assign geten4 to NULL pointer \n");
    exit(1);
  } else {
    printf("Some assignment condition of geten4 not met \n");
    exit(1);
  }
  calc_alignment();
}
template void geten4<double>::assign(const long n, const long m, const long l, const long k, double* pntr);
template void geten4<float>::assign(const long n, const long m, const long l, const long k, float* pntr);
template void geten4<long>::assign(const long n, const long m, const long l, const long k, long* pntr);
template void geten4<int>::assign(const long n, const long m, const long l, const long k, int* pntr);

/*-------------------------------------------------------
   ressign 
	if the geten4 is assigned, reassign it to a new
        location
-------------------------------------------------------*/
template <typename T>
void geten4<T>::reassign(const long n, const long m, const long l, const long k, T* pntr)
{
  const long ll=n*m*l*k;
  const long mm=std::numeric_limits<long>::max();
//  if (m_assigned && ll >= 1 && ll < mm && pntr != NULL) 
  if (m_assigned && ll >= 0 && ll < mm && pntr != NULL) 
  {
    m_buf = pntr;
    m_ptr = pntr;
    m_len = ll;
    m_nd1 = n;
    m_nd2 = m;
    m_nd3 = l;
    m_nd4 = k;
    m_assigned = true;
    m_allocated = false;
  } else if (!m_assigned) {
    printf("Attempted to reassign an unassigned geten4 \n");
    exit(1);
//  } else if (ll < 1) {
//    printf("Attempted to reassign geten4 with m_len < 1 \n");
  } else if (ll < 0) {
    printf("Attempted to reassign geten4 with m_len < 0 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to reassign geten4 > long limit \n");
    exit(1);
  } else if (pntr == NULL) {
    printf("Attempted to reassign geten4 to NULL pointer \n");
    exit(1);
  } else {
    printf("Some reassignment condition of geten4 not met \n");
    exit(1);
  }
  calc_alignment();
}
template void geten4<double>::reassign(const long n, const long m, const long l, const long k, double* pntr);
template void geten4<float>::reassign(const long n, const long m, const long l, const long k, float* pntr);
template void geten4<long>::reassign(const long n, const long m, const long l, const long k, long* pntr);
template void geten4<int>::reassign(const long n, const long m, const long l, const long k, int* pntr);

/*-------------------------------------------------------
   Deallocate
-------------------------------------------------------*/
template <typename T>
void geten4<T>::deallocate()
{
  if (m_allocated)  
  { 
    m_buf = NULL;
    std::free(m_ptr); 
    m_len = 0;
    m_nd1 = 0;
    m_nd2 = 0;
    m_nd3 = 0;
    m_nd4 = 0;
    m_alignment = 0;
    m_allocated = false;
  } else {
    printf("Attempted to deallocate unset geten4 \n");
    exit(1);
  }
}
template void geten4<double>::deallocate();
template void geten4<float>::deallocate();
template void geten4<long>::deallocate();
template void geten4<int>::deallocate();
/*-------------------------------------------------------
   unassign 
-------------------------------------------------------*/
template <typename T>
void geten4<T>::unassign()
{
  if (m_assigned) {
    m_len = 0;
    m_nd1 = 0;
    m_nd2 = 0;
    m_nd3 = 0;
    m_nd4 = 0;
    m_buf = NULL;
    m_ptr = NULL;
    m_assigned = false;
    m_alignment = 0;
  } else {
    printf("Attempted to unassign an unset geten4 \n");
    exit(1);
  }
}
template void geten4<double>::unassign();
template void geten4<float>::unassign();
template void geten4<long>::unassign();
template void geten4<int>::unassign();
/*-------------------------------------------------------
   free 
-------------------------------------------------------*/
template<typename T>
void geten4<T>::free()
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
    m_nd1 = 0;
    m_nd2 = 0;
    m_nd3 = 0;
    m_nd4 = 0;
    m_alignment = 0;
    m_allocated = false;
    m_assigned = false;
  }
}
template void geten4<double>::free();
template void geten4<float>::free();
template void geten4<long>::free();
template void geten4<int>::free();

/*-------------------------------------------------------
   info
    - prints informaiton about the vector
-------------------------------------------------------*/
template <typename T>
void geten4<T>::info() const
{
  if (m_assigned || m_allocated) 
  {
    printf("geten4 has %ld elements \n",m_len);
    printf("geten4 has %ld rank in d1 \n",m_nd1);
    printf("geten4 has %ld rank in d2 \n",m_nd2);
    printf("geten4 has %ld rank in d3 \n",m_nd3);
    printf("geten4 has %ld rank in d4 \n",m_nd4);
    printf("geten4 is aligned to %d bytes \n",m_alignment);
    printf("geten4 points to  %p \n",(void *) m_ptr);
    printf("geten4 buffer starts at  %p \n",(void *) m_buf);
  } else {
    printf("geten4 is unset \n");
    printf("geten4 has %ld elements \n",m_len);
    printf("geten4 has %ld rank in d1 \n",m_nd1);
    printf("geten4 has %ld rank in d2 \n",m_nd2);
    printf("geten4 has %ld rank in d3 \n",m_nd3);
    printf("geten4 has %ld rank in d4 \n",m_nd4);
  }
}
template void geten4<double>::info() const;
template void geten4<float>::info() const;
template void geten4<long>::info() const;
template void geten4<int>::info() const;
/*-------------------------------------------------------
  zero()
	- zeros the matrix 
-------------------------------------------------------*/
template <typename T>
void geten4<T>::zero()
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
template void geten4<double>::zero();
template void geten4<float>::zero();
template void geten4<long>::zero();
template void geten4<int>::zero();
/*-------------------------------------------------------
   =
        operator overloading to set matrix equal to constant
-------------------------------------------------------*/
template <typename T>
void geten4<T>::operator= (const T a)
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
template void geten4<double>::operator= (const double a);
template void geten4<float>::operator= (const float a);
template void geten4<long>::operator= (const long a);
template void geten4<int>::operator= (const int a);
/*-------------------------------------------------------
  print
	- prints elements of vector
-------------------------------------------------------*/
template <>
void geten4<double>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long l=0;l<m_nd4;l++)
  {
    for (long k=0;k<m_nd3;k++)
    {
      for (long j=0;j<m_nd2;j++)
      {
        for (long i=0;i<m_nd1;i++)
        {
          printf("[%ld,%ld,%ld,%ld]    %18.15E \n",i,j,k,l,*(m_buf+xx));
          xx++;
        }
      }
    }
  }
}

template <>
void geten4<float>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long l=0;l<m_nd4;l++)
  {
    for (long k=0;k<m_nd3;k++)
    {
      for (long j=0;j<m_nd2;j++)
      {
        for (long i=0;i<m_nd1;i++)
        {
          printf("[%ld,%ld,%ld,%ld]    %10.7E \n",i,j,k,l,*(m_buf+xx));
          xx++;
        }
      }
    } 
  }
}

template <>
void geten4<long>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long l=0;l<m_nd4;l++)
  {
    for (long k=0;k<m_nd3;k++)
    {
      for (long j=0;j<m_nd2;j++)
      {
        for (long i=0;i<m_nd1;i++)
        {
          printf("[%ld,%ld,%ld,%ld]    %ld \n",i,j,k,l,*(m_buf+xx));
          xx++;
        }
      }
    }
  }
}

template <>
void geten4<int>::print() const
{
  assert(m_allocated||m_assigned);
  long xx=0;
  for (long l=0;l<m_nd4;l++)
  {
    for (long k=0;k<m_nd3;k++)
    {
      for (long j=0;j<m_nd2;j++)
      {
        for (long i=0;i<m_nd1;i++)
        {
          printf("[%ld,%ld,%ld,%ld]    %d \n",i,j,k,l,*(m_buf+xx));
          xx++;
        }
      }
    }
  }
}
