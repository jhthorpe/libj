#!/bin/bash
g++ --std=c++11 test.cpp -llapack -lblas 
#g++ --std=c++11 test.cpp ~/lapack-3.9.0/liblapack.a ~/lapack-3.9.0/librefblas.a
