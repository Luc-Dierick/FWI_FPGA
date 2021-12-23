#ifndef UPDATE_DIRECTION_H
#define UPDATE_DIRECTION_H

#include <hls_stream.h>
#include <ap_fixed.h>
#include <ap_fixed_special.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex>

#define DIM 84

#define ROW DIM
#define COL DIM

#define FREQ 17
#define RECV 17
#define SRC 15

typedef std::complex<float> complex_float;
typedef float float_t;

template<typename T>
class data_struct{
public:
    T data;
    ap_uint<1> last;
};


#endif
