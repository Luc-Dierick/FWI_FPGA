#include "dotProduct.h"

void dotprod(hls::stream<complex_float> &dotprod_in_matrix, hls::stream<reg_float> &dotprod_in_vector, hls::stream<data_struct<dout_t> > &dotprod_out) {
#pragma HLS INTERFACE axis port=dotprod_in_matrix
#pragma HLS INTERFACE axis port=dotprod_in_vector
#pragma HLS INTERFACE axis port=dotprod_out
#pragma HLS INTERFACE ap_ctrl_none port=return


    static complex_float a[ROW][COL];
    float b[COL];
    static complex_float c[ROW];

    int const FACTOR = COL/4;
#pragma HLS array_partition variable=a block factor=FACTOR dim=2
#pragma HLS array_partition variable=b block factor=FACTOR dim=1

    // stream in first matrix
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j <COL; j++) {
#pragma HLS PIPELINE II=1
            a[i][j] = dotprod_in_matrix.read();

        }

    // stream in the vector
    for (int i = 0; i < COL; i++){
#pragma HLS PIPELINE II=1
        b[i] = dotprod_in_vector.read();
    }

    // dot product of matrix A and vector B
    std::complex<float> sum = 0;
	L1:for (int row = 0; row < ROW; ++row){
    L2:for (int col = 0; col < COL; ++col){
		#pragma HLS PIPELINE II=4
    		sum += a[row][col] * b[col];
    	}
    	c[row] = sum;
    	sum = 0;
    }
    // stream out result matrix
    data_struct<dout_t> out_data;

    for (int i = 0; i < ROW; i++)
    {
#pragma HLS PIPELINE II=1
        out_data.data = c[i];
        out_data.last = (i == (ROW - 1)) ? 1 : 0;

        dotprod_out.write(out_data);
    }

    return;
}
