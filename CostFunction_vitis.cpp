#include "CostFunction_vitis.h"


void updateDirection(hls::stream <complex_float> &update_input, hls::stream <complex_float> &update_kappa,
                     hls::stream <complex_float> &update_output) {
#pragma HLS INTERFACE axis port=update_input
#pragma HLS INTERFACE axis port=update_kappa
#pragma HLS INTERFACE axis port=update_output
#pragma HLS INTERFACE ap_ctrl_none port=return

    static complex_float a[ROW];
    static complex_float b[ROW][COL];
    static complex_float c[COL];
    int factor = 2;

#pragma HLS array_partition variable=b cyclic factor=factor dim=2
#pragma HLS array_partition variable=a cyclic factor=30 dim=1

#pragma HLS BIND_STORAGE variable=d type=RAM_1P


    // stream in first matrix
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE II=1
            b[i][j] = update_kappa.read();
        }


    // stream in the vector
    for (int i = 0; i < ROW; i++) {
#pragma HLS PIPELINE II=1
        a[i] = update_input.read();
    }


    complex_float conj;
    L1:
    for (int row = 0; row < ROW; ++row) {
        L2:
        for (int col = 0; col < COL; ++col) {
#pragma HLS PIPELINE II=1
            conj.data.real(b[row][col].data.real() * a[row].data.real() + b[row][col].data.imag() * a[row].data.imag());
            conj.data.imag(
                    -b[row][col].data.real() * a[row].data.imag() - b[row][col].data.imag() * a[row].data.real());
            c[col].data += conj.data;
        }
    }


    for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE II=1
        c[j].last = (j == (COL - 1)) ? 1 : 0;
        update_output.write(c[j]);
        c[j].data = 0;

    }

    return;
}

