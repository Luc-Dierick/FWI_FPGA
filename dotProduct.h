#ifndef DOTPRODUCT_H
#define DOTPRODUCT_H

#include <hls_stream.h>
#include <ap_fixed.h>
#include <ap_fixed_special.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex>

#define DIM 84

typedef std::complex<float> din_t;
typedef float din2_t;
typedef std::complex<float> dout_t;

template<typename T>
class data_struct{
public:
    T data;
    ap_uint<1> last;
};


#endif
