#ifndef UPDATE_DIRECTION_H
#define UPDATE_DIRECTION_H

#include <hls_stream.h>
#include "ap_axi_sdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <complex>

#define ROW 30
#define COL 10

typedef float float_t;
typedef  hls::axis<std::complex<float>,0,0,0> complex_float;

// This is for vivado
//#include <ap_fixed.h>
//typedef std::complex<float> complex_float;
//#include <ap_fixed_special.h>
//template<typename T>
//class data_struct{
//public:
//	ap_uint<1> last;
//    T data;
//
//};

#endif

