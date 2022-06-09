#include "ForwardFunction.h"

void ForwardFunction(hls::stream<complex_float> &kappa_matrix, hls::stream<reg_float> &cur_est_pressure_field, hls::stream<data_struct<dout_t> > &output_vector) {
#pragma HLS INTERFACE axis port=kappa_matrix
#pragma HLS INTERFACE axis port=cur_est_pressure_field
#pragma HLS INTERFACE axis port=output_vector
#pragma HLS INTERFACE ap_ctrl_none port=return


    float b[COL];
    static complex_float a[LOW][COL];
    static complex_float d[HIGH][COL];
    static complex_float c[COL];

#pragma HLS array_partition variable=a cyclic factor=20 dim=2
#pragma HLS array_partition variable=b cyclic factor=10 dim=1
#pragma HLS array_partition variable=d cyclic factor=10 dim=2

#pragma HLS resource variable=d core=RAM_1P_LUTRAM

    // stream in first part of kappa matrix
    for (int i = 0; i < LOW; i++)
        for (int j = 0; j <COL; j++) {
#pragma HLS PIPELINE
            a[i][j] = kappa_matrix.read();

        }
    // stream in second part of kappa matrix
    for (int i = 0; i < HIGH; i++)
        for (int j = 0; j <COL; j++) {
#pragma HLS PIPELINE
            d[i][j] = kappa_matrix.read();

        }

    // stream in the current estimated pressure field vector
    for (int i = 0; i < COL; i++){
#pragma HLS PIPELINE
        b[i] = cur_est_pressure_field.read();
    }

    // Perform the do product of the kappa matrix and current estimated pressure field vector
    std::complex<float> sum = 0;
    L2:for (int col = 0; col < COL; ++col){
    L1:for (int row = 0; row < LOW; ++row){
#pragma HLS PIPELINE
    sum += a[row][col] * b[col];
}
    L3:for (int row = 0; row < HIGH; ++row){
#pragma HLS PIPELINE
    sum += d[row][col] * b[col];
}
    c[col] = sum;
    sum = 0;
}

    // stream out the result vector
    data_struct<dout_t> out_data;

    for (int i = 0; i < COL; i++)
    {
#pragma HLS PIPELINE
        out_data.data = c[i];
        out_data.last = (i == (COL - 1)) ? 1 : 0;

        output_vector.write(out_data);
    }

}
