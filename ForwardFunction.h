#ifndef FORWARD_FUNCTION_H
#define FORWARD_FUNCTION_H

#include <hls_stream.h>
#include <ap_fixed.h>
#include <ap_fixed_special.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex>

#define ROW 300
#define COL 100
#define LOW 200
#define HIGH 100

typedef std::complex<float> complex_float;
typedef float reg_float;
typedef std::complex<float> dout_t;

template<typename T>
class data_struct {
public:
    T data;
    ap_uint<1> last;
};


#endif
