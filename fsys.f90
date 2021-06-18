!--------------------------------------------------------
! fsys
!	- module containing subroutines, types, and 
!	  functions related to constructing, storing
!	  and reading files.
!
!	-  
!--------------------------------------------------------
! Subroutines and functions
! fsys_init(sys)	  : initializes the filesystem
! fsys_print(sys)	  : prints info about filesystem
! fsys_add(sys,name,  	  : adds a file to the system
!          rbyte,fid)         and returns file id
! fsys_close(sys,fid)     : closes a file
! fsys_open(sys,fidm,rw)  : opens a file for read/write
! fsys_close_all(sys,fid) : closes all files
! fsys_dwrite(sys,fid,sr, : writes a real*8 buffer to 
!             N,BUF)          file starting at rec sr
! fsys_iwrite(sys,fid,sr, : writes a int*8 buffer to 
!             N,BUF)          file starting at rec sr
! fsys_dread(sys,fid,sr, : reads a real*8 buffer from
!             N,BUF)          file starting at rec sr
! fsys_iread(sys,fid,sr, : reads a int*8 buffer from
!             N,BUF)          file starting at rec sr

module fsys
  implicit none

!--------------------------------------------------------
! jsys_type
!	- general filesystem type, that stores LISTS
!	  on a set of FILES.
!
!	- note that the limits on the system are 
!	  currently hardcoded, probably could be made
!	  compiler options
!
!--------------------------------------------------------
! FILE info
! file_nmax		: int*8, max number of files
! file_num		: int*8, current number of files
! file_nfree		: int*8, remaining number of files
! file_next		: int*8, index of next file
! file_name(100)	: chr*8, name of files
! file_unit(100)	: int*8, units of files
! file_recbyte(100)	: int*8, record length of file
! file_x8inrec(100)	: int*8, number of 8-byte that fit on rec
! file_nextrec(100)	: int*8, location of next rec
! file_isopen(100)	: bool,  true if file is open

type jsys_type
  integer(kind=8)  :: file_nmax=100
  integer(kind=8)  :: file_num
  integer(kind=8)  :: file_nfree
  integer(kind=8)  :: file_next
  character(len=8) :: file_name(100)
  integer(kind=8)  :: file_unit(100)
  integer(kind=8)  :: file_recbyte(100)
  integer(kind=8)  :: file_x8inrec(100)
  integer(kind=8)  :: file_nextrec(100)
  logical          :: file_isopen(100)
end type jsys_type

contains

!--------------------------------------------------------
! fsys_init
!	- initializes the filesystem
!--------------------------------------------------------
subroutine fsys_init(sys)
  implicit none
  type(jsys_type), intent(inout) :: sys
  
  sys%file_num                      = 0
  sys%file_nfree                    = sys%file_nmax
  sys%file_next                     = 1
  sys%file_name(1:sys%file_nmax)    = '        '
  sys%file_unit(1:sys%file_nmax)    = 10
  sys%file_recbyte(1:sys%file_nmax)  = 1
  sys%file_x8inrec(1:sys%file_nmax) = 0
  sys%file_nextrec(1:sys%file_nmax) = 1
  sys%file_isopen(1:sys%file_nmax)  = .false.

end subroutine fsys_init

!--------------------------------------------------------
! fsys_print
!	- prints information about the filesystem 
!--------------------------------------------------------
! sys			: jsys_type, filesystem
subroutine fsys_print(sys)
  implicit none
  type(jsys_type), intent(in) :: sys
  integer(kind=8) :: i

  write(*,'(1x,A)')    "Filesystem information "
  write(*,'(1x,A,I4)') "Max number of files    :",sys%file_nmax
  write(*,'(1x,A,I4)') "Total files            :",sys%file_num
  write(*,'(1x,A,I4)') "Free files             :",sys%file_nfree

  do i=1,sys%file_num
    write(*,'(1x,A,I4)')  "File number        : ",i
    write(*,'(1x,A,A8)')  "File name          : ",sys%file_name(i)
    write(*,'(1x,A,I4)')  "File unit          : ",sys%file_unit(i) 
    write(*,'(1x,A,L)')   "File is open       : ",sys%file_isopen(i)
    write(*,'(1x,A,I20)') "File Record Length : ",sys%file_recbyte(i)
    write(*,'(1x,A,I20)') "DP elem per record : ",sys%file_x8inrec(i)
    write(*,'(1x,A,I20)') "Next rec on file   : ",sys%file_nextrec(i)
  end do 
  write(*,*)
end subroutine fsys_print

!--------------------------------------------------------
! fsys_add
!	- adds a file to the filesystem, the internal 
!	  file id is returned
!--------------------------------------------------------
! sys			: jsys_type, filesystem
! fname			: char(len8), name of file
! funit			: int*8, file unit for IO
! frlen			: int*8, rec len in bytes 
! fid			: int*8, id in the filelist
subroutine fsys_add(sys,fname,frlen,fid)
  implicit none
  type(jsys_type), intent(inout)  :: sys
  character(len=8), intent(in)    :: fname
  integer(kind=8), intent(in)     :: frlen
  integer(kind=8), intent(inout)  :: fid
  
  !check we have space
  if (sys%file_nfree .lt. 1) then
    write(*,'(A)') "fsys : ERROR ERROR ERROR"
    write(*,'(A)') "There are no free files left"
    STOP 1 
  end if

  !check through existing files for same name or unit
  do fid=1,sys%file_num
    if (sys%file_name(fid) .eq. fname) then
      write(*,'(A)') "fsys : ERROR ERROR ERROR"
      write(*,'(A,A8,A)') "File named ",fname," already exists"
      STOP 1
    end if
  end do 

  !otherwise, add this to the list
  fid = sys%file_next
  sys%file_num = sys%file_num + 1
  sys%file_nfree = sys%file_nfree - 1
  sys%file_next = sys%file_next + 1

  sys%file_name(fid) = fname
  sys%file_unit(fid) = sys%file_unit(fid) + fid
  sys%file_recbyte(fid) = frlen
  sys%file_x8inrec(fid) = frlen/8 !convert from bytes
  
end subroutine fsys_add


!--------------------------------------------------------
! fsys_open
!	- opens a file in the filesystem 
!--------------------------------------------------------
! sys			: jsys_type, filesystem
! id			: int, id of file
! rdwr			: int, 0 - read, 1 - read/write 
subroutine fsys_open(sys,fid,rdwr)
  implicit none
  type(jsys_type), intent(inout) :: sys
  integer(kind=8), intent(in)    :: fid,rdwr

  character(len=8) :: stat

  !check that fid is in filesystem
  if (fid .gt. sys%file_num) then
    write(*,'(A)') "fsys : ERROR ERROR ERROR"
    write(*,'(A)') "Attempted to open file not in filesystem"
    STOP 1
  end if

  !Set status to the correct thing. 
  if (rdwr .eq. 0) then 
    stat='old     '
  else if (rdwr .eq. 1) then
    stat='replace '
  else 
    write(*,'(A)') "fsys : ERROR ERROR ERROR"
    write(*,'(A)') "Bad input into rdwr of fsys_open"
    STOP 1
  end if

  if (.not. sys%file_isopen(fid)) then 
    open(file=trim(sys%file_name(fid)),unit=sys%file_unit(fid), &
         status=trim(stat),form='unformatted',access='direct', &
         recl=sys%file_recbyte(fid))
    sys%file_isopen(fid) = .true.
  end if

end subroutine fsys_open

!--------------------------------------------------------
! fsys_close
!	- closes a file in the filesystem 
!--------------------------------------------------------
! sys			: jsys_type, filesystem
! id			: int, id of file
subroutine fsys_close(sys,fid)
  implicit none
  type(jsys_type), intent(inout) :: sys
  integer(kind=8), intent(in)    :: fid

  !check that fid is in filesystem
  if (fid .gt. sys%file_num) then
    write(*,'(A)') "fsys : ERROR ERROR ERROR"
    write(*,'(A)') "Attempted to close file not in filesystem"
    STOP 1
  end if

  if (sys%file_isopen(fid)) then
    close(unit=sys%file_unit(fid))
    sys%file_isopen(fid) = .false.
  end if

end subroutine fsys_close

!--------------------------------------------------------
! fsys_close_all
!	- closes all files in the filesystem 
!--------------------------------------------------------
! sys			: jsys_type, filesystem
subroutine fsys_close_all(sys)
  implicit none
  type(jsys_type), intent(inout) :: sys
  integer(kind=8) :: fid

  do fid=1,sys%file_num
    if (sys%file_isopen(fid)) then
      close(unit=sys%file_unit(fid))
      sys%file_isopen(fid) = .false. 
    end if
  end do 

end subroutine fsys_close_all

!--------------------------------------------------------
! fsys_dwrite
!	- writes a dp buffer to a file
!	  in the filesystem, starting at record sr
!
!	- note that this does NOT check that the file
!	  is open, in order to prevent a bunch of 
!	  if statements during long writes
!--------------------------------------------------------
! sys		: jsys_type, filesystem
! fid		: int*8, id of file
! sr		: int*8, starting rec
! N		: int*8, length of buffer
! BUF(N)	: real*8, buffer
subroutine fsys_dwrite(sys,fid,sr,N,BUF)
  implicit none
  type(jsys_type), intent(inout) :: sys
  integer(kind=8), intent(in)    :: fid,N,sr
  real(kind=8), intent(in)       :: BUF(N)

  integer(kind=8) :: i0,i1,i2,funit
  integer(kind=8) :: nn,ll,nl


  funit=sys%file_unit(fid)
  ll = sys%file_x8inrec(fid) !num dp per rec
  nl = (N + ll - 1)/ll       !number of rec 
  nn = sr + nl               

  !Fancy infinite loop, which performs 3 less
  !  operations than a traditional do-loop. Helps
  !  in the case of small writes 
  !This writes all the elements except the last
  i0 = sr 
  i1 = 1  !location in buffer
  i2 = ll !location in buffer
  do while (.true.)
    write(funit,rec=i0) BUF(i1:i2)
    if (i0 .ge. nn-1) exit
    i0 = i0 + 1
    i1 = i1 + ll
    i2 = i2 + ll
  end do 

  !fill in the last element
  i0 = i0 + 1
  i1 = i1 + ll
  i2 = N 
  write(funit,rec=i0) BUF(i1:i2)

end subroutine fsys_dwrite

!--------------------------------------------------------
! fsys_iwrite
!	- writes a i8 buffer to a file  
!	  in the filesystem starting at rec sr
!
!	- note that this does NOT check that the file
!	  is open, in order to prevent a bunch of 
!	  if statements during long writes
!--------------------------------------------------------
! sys		: jsys_type, filesystem
! fid		: int*8, id of file
! sr		: int*8, starting rec
! N		: int*8, length of buffer
! BUF(N)	: int*8, buffer
subroutine fsys_iwrite(sys,fid,sr,N,BUF)
  implicit none
  type(jsys_type), intent(inout) :: sys
  integer(kind=8), intent(in)    :: fid,N,sr
  integer(kind=8), intent(in)    :: BUF(N)

  integer(kind=8) :: i0,i1,i2,funit
  integer(kind=8) :: nn,ll,nl

  funit=sys%file_unit(fid)
  ll = sys%file_x8inrec(fid) !num dp per rec
  nl = (N + ll - 1)/ll       !number of rec 
  nn = sr + nl               

  !Fancy infinite loop, which performs 3 less
  !  operations than a traditional do-loop. Helps
  !  in the case of small writes 
  !This writes all the elements except the last
  i0 = sr 
  i1 = 1  !location in buffer
  i2 = ll !location in buffer
  do while (.true.)
    write(funit,rec=i0) BUF(i1:i2)
    if (i0 .ge. nn-1) exit
    i0 = i0 + 1
    i1 = i1 + ll
    i2 = i2 + ll
  end do 
  
  !fill in the last element
  i0 = i0 + 1
  i1 = i1 + ll
  i2 = N 
  write(funit,rec=i0) BUF(i1:i2)

end subroutine fsys_iwrite

!--------------------------------------------------------
! fsys_dread
!	- reads a dp buffer from the filesystem starting
!	  at a particular record number
!
!	- note that this does NOT check that the file
!	  is open, in order to prevent a bunch of 
!	  if statements during long writes
!--------------------------------------------------------
! sys		: jsys_type, filesystem
! fid		: int*8, id of file
! sr		: int*8, starting record number
! N		: int*8, length of buffer
! BUF(N)	: real*8, buffer
subroutine fsys_dread(sys,fid,sr,N,BUF)
  implicit none
  type(jsys_type), intent(in)    :: sys
  integer(kind=8), intent(in)    :: fid,sr,N
  real(kind=8), intent(inout)    :: BUF(N)

  integer(kind=8) :: i0,i1,i2,funit
  integer(kind=8) :: nn,ll,nl

  funit=sys%file_unit(fid)
  ll = sys%file_x8inrec(fid) !num dp per rec
  nl = (N + ll - 1)/ll       !number of rec 
  nn = sr + nl               

  !Fancy infinite loop, which performs 3 less
  !  operations than a traditional do-loop. Helps
  !  in the case of small writes 
  !This writes all the elements except the last
  i0 = sr 
  i1 = 1  !location in buffer
  i2 = ll !location in buffer
  do while (.true.)
    read(funit,rec=i0) BUF(i1:i2)
    if (i0 .ge. nn-1) exit
    i0 = i0 + 1
    i1 = i1 + ll
    i2 = i2 + ll
  end do 
  
  !fill in the last element
  i0 = i0 + 1
  i1 = i1 + ll
  i2 = N 
  read(funit,rec=i0) BUF(i1:i2)

end subroutine fsys_dread

!--------------------------------------------------------
! fsys_iread
!	- reads an i8 buffer from the filesystem given
!	  a particular file and a starting record
!
!	- note that this does NOT check that the file
!	  is open, in order to prevent a bunch of 
!	  if statements during long writes
!--------------------------------------------------------
! sys		: jsys_type, filesystem
! fid		: int*8, id of file
! sr		: int*8, starting record
! N		: int*8, length of buffer
! BUF(N)	: int*8, buffer
subroutine fsys_iread(sys,fid,sr,N,BUF)
  implicit none
  type(jsys_type), intent(in)    :: sys
  integer(kind=8), intent(in)    :: fid,sr,N
  integer(kind=8), intent(inout) :: BUF(N)

  integer(kind=8) :: i0,i1,i2,funit
  integer(kind=8) :: nn,ll,nl

  funit=sys%file_unit(fid)
  ll = sys%file_x8inrec(fid) !num dp per rec
  nl = (N + ll - 1)/ll       !number of rec 
  nn = sr + nl               

  !Fancy infinite loop, which performs 3 less
  !  operations than a traditional do-loop. Helps
  !  in the case of small writes 
  !This writes all the elements except the last
  i0 = sr 
  i1 = 1  !location in buffer
  i2 = ll !location in buffer
  do while (.true.)
    read(funit,rec=i0) BUF(i1:i2)
    if (i0 .ge. nn-1) exit
    i0 = i0 + 1
    i1 = i1 + ll
    i2 = i2 + ll
  end do 
  
  !fill in the last element
  i0 = i0 + 1
  i1 = i1 + ll
  i2 = N 
  read(funit,rec=i0) BUF(i1:i2)

end subroutine fsys_iread
!--------------------------------------------------------


end module fsys
!--------------------------------------------------------
