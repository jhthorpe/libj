/*---------------------------------------------------------------
  linal.hpp
    JHT July 17, 2021 : created

  - linear algebra subroutines and functions, 
    as well as interfaces to F77 BLAS and LAPACK subroutines


    General naming scheme:
    D,X,Y	is a diagonal matrix
    A,B,C	are rectangular matrices
    U,L		are upper,lower symmetric matrices
    T		indicates a transpose applied to the matrix before
    x,y,z	are vectors
    p		indicates a plus

    When dimensions are given, they are always listed with the 
    dimensions of the FINAL matrix

---------------------------------------------------------------*/
#ifndef LINAL_HPP
#define LINAL_HPP

//interface files
#include "blas_interface.hpp"
#include "lapack_interface.hpp"

//compiler time defintions
#include "linal_def.hpp"

//named correctly
#include "linal_UApB.hpp"
#include "linal_AUBpD.hpp"
#include "linal_AUBpC.hpp"
#include "linal_AUBpY.hpp"
#include "linal_ATpB.hpp"
#include "linal_AUxpy.hpp"
#include "linal_ATUxpy.hpp"
#include "linal_ATBpU.hpp"
#include "linal_ATDAeU.hpp"
#include "linal_ATDAeY.hpp"
#include "linal_ATDApU.hpp"
#include "linal_diag_ABpC.hpp"
#include "linal_ATUpB.hpp"
#include "linal_DATpB.hpp"
#include "linal_ATBpC.hpp"
#include "linal_ABpC.hpp"
#include "linal_DApB.hpp"

//these are not named correctly
#include "linal_usym2v.hpp"
#include "linal_geprint.hpp"
#include "linal_svd.hpp"
#include "linal_usym3_invrt.hpp"
#include "linal_usym3_usym3_MM.hpp"
#include "linal_usym3_sqm3_MM_UP.hpp"

//some things that are probably not needed
//And are named incorrecly
#include "linal_scal_small.hpp"
#include "linal_MTM_UP_small.hpp"
#include "linal_vxv_small.hpp"
#include "linal_vxM_small.hpp"

#endif
