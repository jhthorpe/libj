!--------------------------------------------------------
! dmem
!	- module containing subroutines and 
!	  data types related to a double precsision
!	  buffer 
!
!	- this is currently just a linear buffer, without
!	  any named sections
!
! Subroutines
! dmem_init(N,META)     : initializes memory structure
! dmem_reserve(N,META)	: reserves a section of memory
! dmem_free(N,META)	: frees a section of memory
! dmem_info(META)	: prints info about struct 
! dmem_checkout(N,I0,	: reserves a section of memory
!		META)	  and returns it's loc in DBUF
!
!--------------------------------------------------------
module dmem

!---------------------------------------------------
! dmem_type
!       - type containing info about
!         double precision memory buffer
!---------------------------------------------------
! ntot            : int*8,total number of elements  
! nfree           : int*8,remaining free elements 
! next            : int*8,index of next free element

type dmem_type
  integer(kind=8) :: ntot
  integer(kind=8) :: nfree
  integer(kind=8) :: next
end type dmem_type

  
contains

!--------------------------------------------------------
! dmem_init
!	- initializes a double precision 
!	  buffer and it's data structure. 
!
!--------------------------------------------------------
! N		: int*8, number of elements
! META		: dmem_type, meta data
subroutine dmem_init(N,META)
  implicit none
  integer(kind=8), intent(in) :: N
  type(dmem_type), intent(inout) :: META

  META%ntot  = N
  META%nfree = N
  META%next  = 1

  write(*,*) "initializing ",N,"elements"

end subroutine dmem_init

!--------------------------------------------------------
! dmem_reserve
!	- reserves a new section of memoruy
!--------------------------------------------------------
! NTAKE		: int*8, number of elements to reserve
! META		: dmem_type, meta data
subroutine dmem_reserve(NTAKE,META)
  implicit none
  integer(kind=8), intent(in) :: NTAKE
  type(dmem_type), intent(inout) :: META

  if (META%nfree .lt. NTAKE) then
    write(*,*) "JLIB : ERROR ERROR ERROR"
    write(*,*) "Attempted to reserve more memory than was available"
    stop 1
  end if

  META%nfree = META%nfree - NTAKE
  META%next  = META%next + NTAKE

end subroutine dmem_reserve

!--------------------------------------------------------
! dmem_free
!	- frees a section of memory
!--------------------------------------------------------
! NRTRN		: int*8, number of elements to free 
! META		: dmem_type, meta data
subroutine dmem_free(NRTRN,META)
  implicit none
  integer(kind=8), intent(in) :: NRTRN
  type(dmem_type), intent(inout) :: META

  if (META%ntot .lt. META%nfree+NRTRN) then
    write(*,*) "JLIB : ERROR ERROR ERROR"
    write(*,*) "Attempted to free more memory than BUF was given" 
    stop 1
  end if

  META%nfree = META%nfree + NRTRN
  META%next  = META%next - NRTRN
end subroutine dmem_free

!--------------------------------------------------------
! dmem_info
!	- frees a section of memory
!--------------------------------------------------------
! META		: dmem_type, meta data
subroutine dmem_info(META)
  implicit none
  type(dmem_type), intent(inout) :: META 
  
  write(*,'(1x,A)') "DP buff has "
  write(*,'(1x,I20,A)') META%ntot," elements" 
  write(*,'(1x,I20,A)') META%nfree," free elements"
  write(*,'(1x,I20,A)') META%next," is next element"
!  write(*,'(1x,I20,A)') loc(META), " is the address of metadata" 

end subroutine dmem_info

!--------------------------------------------------------
! dmem_checkout
!	- checks out a section of memory at the end of
!	  current array and returns its address
!--------------------------------------------------------
! N		: int*8, number of elements to reserve
! I0		: int*8, address in DBUF
! META		: dmem_type, meta data
subroutine dmem_checkout(N,I0,META)
  implicit none
  integer(kind=8), intent(in) :: N
  integer(kind=8), intent(inout) :: I0
  type(dmem_type), intent(inout) :: META

  if (META%nfree .lt. N) then
    write(*,*) "JLIB : ERROR ERROR ERROR"
    write(*,*) "Attempted to reserve more memory than was available"
    stop 1
  end if

  I0 = META%next
  META%nfree = META%nfree - N
  META%next  = META%next + N

end subroutine dmem_checkout

end module dmem
!--------------------------------------------------------
