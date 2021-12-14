//
// Created by Luc on 09/12/2021.
//

#ifndef FWI_DOTPRODUCT_H
#define FWI_DOTPRODUCT_H

#define N 100
#define MAGNITUDE 10 //17*15*15
#define GRIDPOINTS 10

#include <complex>

void dotProd(std::complex<double> A[MAGNITUDE][GRIDPOINTS], double B[MAGNITUDE], std::complex<double> out[MAGNITUDE]);


#endif //FWI_DOTPRODUCT_H
