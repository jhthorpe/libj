#!/bin/bash
opts="--std=c++11 -O3 -flto -funroll-loops"
g++ $opts -c dcore.cpp
g++ $opts -c dvec.cpp
g++ $opts test.cpp *.o
