#include "ForwardFunction.h"

void ForwardFunction(hls::stream<complex_float> &kappa_matrix, hls::stream<reg_float> &cur_est_pressure_field, hls::stream<data_struct<dout_t> > &output_vector) {
#pragma HLS INTERFACE axis port=kappa_matrix
#pragma HLS INTERFACE axis port=cur_est_pressure_field
#pragma HLS INTERFACE axis port=output_vector
#pragma HLS INTERFACE ap_ctrl_none port=return


    static complex_float a[COL];
    float b[COL];
    static complex_float c[ROW];
    // stream out result matrix
    data_struct<dout_t> out_data;

//
int const FACTOR = 10;
#pragma HLS array_partition variable=a cyclic factor=FACTOR dim=1
#pragma HLS array_partition variable=b cyclic factor=FACTOR dim=1
#pragma HLS RESOURCE variable=a core=RAM_1P_LUTRAM
    // stream in the vector
    for (int i = 0; i < COL; i++){
#pragma HLS PIPELINE II=1
        b[i] = cur_est_pressure_field.read();
    }

  for(int t = 0; t< ROW; t++){
    // stream in first matrix
        for (int j = 0; j <COL; j++) {
#pragma HLS PIPELINE II=1
            a[j] = kappa_matrix.read();
        }


    // dot product of matrix A and vector B
    L2:for (int col = 0; col < COL; ++col){
		#pragma HLS PIPELINE II=1
    		c[t] += a[col] * b[col];

    }
  }
  for (int i =0; i < ROW; i++){
	 out_data.data = c[i];
	 out_data.last = (i == (ROW - 1)) ? 1 : 0;
	 output_vector.write(out_data);
  }
}
