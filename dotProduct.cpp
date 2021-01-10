#include "dotProduct.h"

void dotprod(hls::stream<din_t> &dotprod_in_matrix, hls::stream<din2_t> &dotprod_in_vector, hls::stream<data_struct<dout_t> > &dotprod_out) {
#pragma HLS INTERFACE axis port=dotprod_in_matrix
#pragma HLS INTERFACE axis port=dotprod_in_vector
#pragma HLS INTERFACE axis port=dotprod_out
#pragma HLS INTERFACE ap_ctrl_none port=return


    static din_t a[DIM][DIM];
    float b[DIM];
    static din_t c[DIM];

    int const FACTOR = DIM/4;
#pragma HLS array_partition variable=a block factor=FACTOR dim=2
#pragma HLS array_partition variable=b block factor=FACTOR dim=1

    // stream in first matrix
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++) {
#pragma HLS PIPELINE II=1
            a[i][j] = dotprod_in_matrix.read();

        }

    // stream in the vector
    for (int i = 0; i < DIM; i++){
#pragma HLS PIPELINE II=1
        b[i] = dotprod_in_vector.read();
    }

    // dot product of matrix A and vector B
    std::complex<float> sum = 0;
    L1:for (int ia = 0; ia < DIM; ++ia){
    L2:for (int ib = 0; ib < DIM; ++ib)
{
#pragma HLS PIPELINE II=4
    sum += a[ia][ib] * b[ib];
}
    c[ia] = sum;
    sum = 0;
}
    // stream out result matrix
    data_struct<dout_t> out_data;

    for (int i = 0; i < DIM; i++)
    {
#pragma HLS PIPELINE II=1
        out_data.data = c[i];
        out_data.last = (i == (DIM - 1)) ? 1 : 0;

        dotprod_out.write(out_data);
    }

    return;
}
