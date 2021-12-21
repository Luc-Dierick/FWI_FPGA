#include <hls_stream.h>
#include <ap_fixed.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 84

typedef float din_t;
typedef float dout_t;

template<typename T>
class data_struct{
public:
    T data;
    ap_uint<1> last;
};
