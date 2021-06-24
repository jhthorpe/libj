program test_omp
  use omp_lib
  implicit none
  integer :: i

  !$OMP parallel
  write(*,*) "Hello from process",omp_get_thread_num() 
  !$OMP end parallel

  !$OMP parallel 
  !$OMP single
  do i=1,10
  !$OMP task
    call foo(i,omp_get_thread_num())
  !$OMP end task
  end do 
  !$OMP end single
  !$OMP end parallel

  write(*,*)
  write(*,*) 

  !$OMP parallel 
  !$OMP single
  !$OMP task
  do i=1,10
    call foo(i,omp_get_thread_num())
  end do 
  !$OMP end task
  !$OMP end single
  !$OMP end parallel


end program test_omp

subroutine foo(ii,nt)
  implicit none
  integer :: ii,nt
 
  write(*,*) ii,nt


end subroutine foo
