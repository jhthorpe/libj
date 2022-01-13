/*-------------------------------------------------------
  vec.cpp
    JHT, October 27, 2021 : created

  .cpp file for vector templates

-------------------------------------------------------*/
#include "vec.hpp"

/*-------------------------------------------------------
  Constructors
-------------------------------------------------------*/
template<typename T>
vec<T>::vec() 
{
  m_ptr = NULL;
  m_buf = NULL;
  m_len = 0;
  m_allocated = false;
  m_assigned = false;
  m_alignment = 0;
}
template vec<double>::vec();
template vec<float>::vec();
template vec<long>::vec();
template vec<int>::vec();

template<typename T>
vec<T>::vec(const long n)
{
  m_assigned = false;
  m_allocated = false;
  allocate(n);
}
template vec<double>::vec(const long n);
template vec<float>::vec(const long n);
template vec<long>::vec(const long n);
template vec<int>::vec(const long n);

template <typename T>
vec<T>::vec(const long n, T* ptr)
{
  m_assigned = false;
  m_allocated = false;
  assign(n,ptr);
}
template vec<double>::vec(const long n, double* ptr);
template vec<float>::vec(const long n, float* ptr);
template vec<long>::vec(const long n, long* ptr);
template vec<int>::vec(const long n, int* ptr);
/*-------------------------------------------------------
  Destructors 
-------------------------------------------------------*/
template <typename T>
vec<T>::~vec()
{ 
  free();
}
template vec<double>::~vec();
template vec<float>::~vec();
template vec<long>::~vec();
template vec<int>::~vec();

/*-------------------------------------------------------
 * int calc_alignment()
 * calculates the alignment of m_buf
-------------------------------------------------------*/
template<typename T>
void vec<T>::calc_alignment()
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
template void vec<double>::calc_alignment();
template void vec<float>::calc_alignment();
template void vec<long>::calc_alignment();
template void vec<int>::calc_alignment();

/*-------------------------------------------------------
  Allocate 
    - uses malloc to allocate an double prescision 
      vector
-------------------------------------------------------*/
template<typename T>
void vec<T>::allocate(const long n)
{
  const long mm=std::numeric_limits<long>::max();
//  if (!(m_allocated || m_assigned) && n >= 1 && n <= mm) 
  if (!(m_allocated || m_assigned) && n >= 0 && n <= mm) 
  {
    m_ptr = (T*) malloc(n*sizeof(T));
  } else if (n < 0) {
    printf("Attempted to allocate vec of < 0 element \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to allocated vec > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to allocate an already set vec \n");
    exit(1);
  }

  //Check that malloc worked
  if (m_ptr != NULL) 
  {
    m_len = n;
    m_buf = m_ptr;
    m_allocated = true; 
    m_assigned = false;
  } else {
    printf("malloc failed for vec of %ld elements \n",n);
    exit(1);
  }

  calc_alignment();
}
template void vec<double>::allocate(const long n);
template void vec<float>::allocate(const long n);
template void vec<long>::allocate(const long n);
template void vec<int>::allocate(const long n);

/*-------------------------------------------------------
  Alligned_alloc
    - uses malloc to allocate an double prescision 
      vector aligned to some ALIGMENT BYTES
-------------------------------------------------------*/
template <typename T>
void vec<T>::aligned_allocate(const int ALIGN, const long N)
{
  const long mm=std::numeric_limits<long>::max();
//  if (!(m_allocated || m_assigned) && N >= 1 && N <= mm) 
  if (!(m_allocated || m_assigned) && N >= 0 && N <= mm) 
  {
    m_ptr = (T*) malloc(ALIGN+N*sizeof(T));
//  } else if (N < 1) {
//    printf("Attempted to aligned allocate vec of < 1 element \n");
  } else if (N < 0) {
    printf("Attempted to aligned allocate vec of < 0 element \n");
    exit(1);
  } else if (N > mm) {
    printf("Attempted to aligned allocated vec > long limit \n");
    exit(1);
  } else if (m_allocated || m_assigned) {
    printf("Attempted to aligned allocate an already set vec \n");
    exit(1);
  }

  //now, align the buffer pointer
  if (m_ptr != NULL)
  {
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
    m_len = N;
    m_allocated = true;
    m_assigned = false;

  } else {
    printf("aligned_allocate malloc of vec failed \n");
    exit(1);
  }
  //calculate max alignment
  calc_alignment();
}
template void vec<double>::aligned_allocate(const int ALIGN, const long N);
template void vec<float>::aligned_allocate(const int ALIGN, const long N);
template void vec<long>::aligned_allocate(const int ALIGN, const long N);
template void vec<int>::aligned_allocate(const int ALIGN, const long N);
/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
template <typename T>
void vec<T>::assign(const long n, T* ptr)
{
  const long mm=std::numeric_limits<long>::max();
  if (!(m_allocated || m_assigned) && n >= 0 && n < mm && ptr != NULL) 
//  if (!(m_allocated || m_assigned) && n >= 1 && n < mm && ptr != NULL) 
  {
    m_ptr = ptr;
    m_buf = ptr;
    m_len = n;
    m_assigned = true;
    m_allocated = false;
  } else if (m_allocated || m_assigned) {
    printf("Attempted to assign an already set vec \n");
    exit(1);
//  } else if (n < 1) {
//    printf("Attempted to assign vec with len < 1 \n");
  } else if (n < 0) {
    printf("Attempted to assign vec with len < 0 \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to assign vec > long limit \n");
    exit(1);
  } else if (ptr == NULL) {
    printf("Attempted to assign vec to NULL pointer \n");
    exit(1);
  } else {
    printf("Some assignment condition to vec failed \n");
    exit(1);
  }

  calc_alignment();
}
template void vec<double>::assign(const long n, double* ptr);
template void vec<float>::assign(const long n, float* ptr);
template void vec<long>::assign(const long n, long* ptr);
template void vec<int>::assign(const long n, int* ptr);

/*-------------------------------------------------------
   reassign 
	- points the vector to a new location in memory
-------------------------------------------------------*/
template <typename T>
void vec<T>::reassign(const long n, T* ptr)
{
  const long mm=std::numeric_limits<long>::max();
//  if (m_assigned && n >= 1 && n < mm && ptr != NULL) 
  if (m_assigned && n >= 0 && n < mm && ptr != NULL) 
  {
    m_ptr = ptr;
    m_buf = ptr;
    m_len = n;
    m_assigned = true;
    m_allocated = false;
  } else if (!m_assigned) {
    printf("Attempted to reassign an unassigned vec \n");
    exit(1);
//  } else if (n < 1) {
//    printf("Attempted to reassign vec with len < 1 \n");
  } else if (n < 0) {
    printf("Attempted to reassign vec with len < 0 \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to reassign vec > long limit \n");
    exit(1);
  } else if (ptr == NULL) {
    printf("Attempted to reassign vec to NULL pointer \n");
    exit(1);
  } else {
    printf("Some reassignment condition to vec failed \n");
    exit(1);
  }

  calc_alignment();
}
template void vec<double>::reassign(const long n, double* ptr);
template void vec<float>::reassign(const long n, float* ptr);
template void vec<long>::reassign(const long n, long* ptr);
template void vec<int>::reassign(const long n, int* ptr);
/*-------------------------------------------------------
   Free 
-------------------------------------------------------*/
template <typename T>
void vec<T>::free()
{
  if (m_allocated)
  { 
    deallocate(); 
  } else if (m_assigned) {
    unassign();
  } else {
    m_ptr = NULL;
    m_buf = NULL;
    m_len = 0;
    m_allocated = false;
    m_assigned = false;
    m_alignment = 0; 
  }
}
template void vec<double>::free();
template void vec<float>::free();
template void vec<long>::free();
template void vec<int>::free();

/*-------------------------------------------------------
   Deallocate
-------------------------------------------------------*/
template <typename T>
void vec<T>::deallocate()
{
  if (m_allocated)  
  { 
    m_len = 0;
    std::free(m_ptr); 
    m_buf = NULL;
    m_allocated = false;
  } else {
    printf("Attempted to deallocate unset vec \n");
    exit(1);
  }
}
template void vec<double>::deallocate();
template void vec<float>::deallocate();
template void vec<long>::deallocate();
template void vec<int>::deallocate();

/*-------------------------------------------------------
   unassign 
-------------------------------------------------------*/
template <typename T>
void vec<T>::unassign()
{
  if (m_assigned) {
    m_ptr = NULL;
    m_buf = NULL;
    m_len = 0;
    m_assigned = false;
    m_alignment = 1;
  } else {
    printf("Attempted to unassign an unset vec \n");
    exit(1);
  }
}
template void vec<double>::unassign();
template void vec<float>::unassign();
template void vec<long>::unassign();
template void vec<int>::unassign();

/*-------------------------------------------------------
   info
    - prints informaiton about the vector
-------------------------------------------------------*/
template <typename T>
void vec<T>::info() const
{
  if (m_assigned || m_allocated) 
  {
    printf("vec has %ld elements \n",m_len);
    printf("vec points to  %p \n",(void*)m_ptr);
    printf("vec buffer at  %p \n",(void*)m_buf);
    printf("vec is aligned to %d BYTES \n",m_alignment); 
  } else {
    printf("vec is unset \n");
    printf("vec has %ld elements \n",m_len);
  }
}
template void vec<double>::info() const;
template void vec<float>::info() const;
template void vec<long>::info() const;
template void vec<int>::info() const;


/*-------------------------------------------------------
   zero 
	- zeros the vector. Uses a fancy inf. loop to 
	  do one less action per iteration
-------------------------------------------------------*/
template <typename T>
void vec<T>::zero()
{
  assert (m_allocated||m_assigned);
  long i=0;
  const long ll=m_len-1;
  while (true)
  {
    *(m_buf+i) = (T) 0;
    if (i >= ll) break;
    i++;
  }
}
template void vec<double>::zero();
template void vec<float>::zero();
template void vec<long>::zero();
template void vec<int>::zero();

/*-------------------------------------------------------
   = 
	overloads the = operator to set vector equal
        to a constant
-------------------------------------------------------*/
template <typename T>
void vec<T>::operator= (const T a)
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
template void vec<double>::operator=(const double a);
template void vec<float>::operator=(const float a);
template void vec<long>::operator=(const long a);
template void vec<int>::operator=(const int a);

/*-------------------------------------------------------
   print
	- prints elements of vector 
-------------------------------------------------------*/
template <>
void vec<double>::print() const
{
  long i=0;
  while (true)
  {
    if (i > m_len-1) break; 
    printf("[%ld]    %18.15E \n",i,*(m_buf+i)); 
    i++;
  }
}
template <>
void vec<long>::print() const
{
  long i=0;
  while (true)
  {
    if (i > m_len-1) break; 
    printf("[%ld]    %ld \n",i,*(m_buf+i)); 
    i++;
  }
}
template <>
void vec<float>::print() const
{
  long i=0;
  while (true)
  {
    if (i > m_len-1) break; 
    printf("[%ld]    %10.7E \n",i,*(m_buf+i)); 
    i++;
  }
}
template <>
void vec<int>::print() const
{
  long i=0;
  while (true)
  {
    if (i > m_len-1) break; 
    printf("[%ld]    %d \n",i,*(m_buf+i)); 
    i++;
  }
}

template<>
void vec<double>::print(const long lo, const long hi) const
{
  long ll = (lo >= 0) ? lo : 0;
  long hh = (hi <= (m_len-1)) ? hi : (m_len-1);
  long i=ll;
  while (true)
  {
    if (i > hh) break; 
    printf("[%ld]    %18.15E \n",i,*(m_buf+i)); 
    i++;
  }
}
template<>
void vec<float>::print(const long lo, const long hi) const
{
  long ll = (lo >= 0) ? lo : 0;
  long hh = (hi <= (m_len-1)) ? hi : (m_len-1);
  long i=ll;
  while (true)
  {
    if (i > hh) break; 
    printf("[%ld]    %10.7E \n",i,*(m_buf+i)); 
    i++;
  }
}

template<>
void vec<long>::print(const long lo, const long hi) const
{
  long ll = (lo >= 0) ? lo : 0;
  long hh = (hi <= (m_len-1)) ? hi : (m_len-1);
  long i=ll;
  while (true)
  {
    if (i > hh) break; 
    printf("[%ld]    %ld \n",i,*(m_buf+i)); 
    i++;
  }
}

template<>
void vec<int>::print(const long lo, const long hi) const
{
  long ll = (lo >= 0) ? lo : 0;
  long hh = (hi <= (m_len-1)) ? hi : (m_len-1);
  long i=ll;
  while (true)
  {
    if (i > hh) break; 
    printf("[%ld]    %d \n",i,*(m_buf+i)); 
    i++;
  }
}
