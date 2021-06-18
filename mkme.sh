#!/bin/bash
gfortran -c dmem.f90
gfortran -c fsys.f90
gfortran -c jlib.f90
gfortran test.f90 *.o -fdefault-integer-8 
