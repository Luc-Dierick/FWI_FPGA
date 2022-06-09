#include "CostFunction.h"

void CostFunction(hls::stream <complex_float> &residual_vector, hls::stream <complex_float> &kappa_matrix,
                  hls::stream <data_struct<complex_float>> &output_vector) {
#pragma HLS INTERFACE axis port=residual_vector
#pragma HLS INTERFACE axis port=kappa_matrix
#pragma HLS INTERFACE axis port=output_vector
#pragma HLS INTERFACE ap_ctrl_none port=return

    static complex_float a[ROW];
    static complex_float b[LOW][COL];
    static complex_float d[HIGH][COL];
    static complex_float c[COL];

    int factor = 25;

//#pragma HLS array_partition variable=d cyclic factor=factor dim=1
#pragma HLS array_partition variable=b cyclic factor=factor dim=2
#pragma HLS array_partition variable=a cyclic factor=factor dim=1
#pragma HLS resource variable=d core=RAM_1P_LUTRAM

    // stream in first part of kappa matrix
    for (int i = 0; i < LOW; i++)
        for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE
            b[i][j] = kappa_matrix.read();
        }

    // stream in second part of kappa matrix
    for (int i = 0; i < HIGH; i++)
        for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE
            d[i][j] = kappa_matrix.read();
        }


    // stream in the residual vector
    for (int i = 0; i < ROW; i++) {
#pragma HLS PIPELINE
        a[i] = residual_vector.read();
    }


// Perform operations to update the direction of the Conjugate Gradient Descent.
    std::complex<float> conj;
    L1:
    for (int row = 0; row < LOW; ++row) {
        L2:
        for (int col = 0; col < COL; ++col) {
#pragma HLS PIPELINE
            conj.real(b[row][col].real() * a[row].real() + b[row][col].imag() * a[row].imag());
            conj.imag(-b[row][col].real() * a[row].imag() - b[row][col].imag() * a[row].real());
            c[col] += conj;
        }
    }

    L3:
    for (int row = 0; row < HIGH; ++row) {
        L4:
        for (int col = 0; col < COL; ++col) {
#pragma HLS PIPELINE
            conj.real(d[row][col].real() * a[LOW + row].real() + d[row][col].imag() * a[LOW + row].imag());
            conj.imag(-d[row][col].real() * a[LOW + row].imag() - d[row][col].imag() * a[LOW + row].real());
            c[col] += conj;
        }
    }

    // stream out the result vector
    data_struct<complex_float> out_data;

    for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE
        out_data.data = c[j];
        out_data.last = (j == (COL - 1)) ? 1 : 0;
        c[j] = 0;
        output_vector.write(out_data);
    }
    return;
}
