#include "updateDirection.h"

void updateDirection(hls::stream<complex_float> &update_input, hls::stream<complex_float> &update_kappa, hls::stream<data_struct<complex_float> > &update_output) {
#pragma HLS INTERFACE axis port=update_input
#pragma HLS INTERFACE axis port=update_kappa
#pragma HLS INTERFACE axis port=update_output
#pragma HLS INTERFACE ap_ctrl_none port=return

    static complex_float a[ROW];
    static complex_float b[ROW][COL];
    static complex_float c[ROW][COL];

    int const FACTOR = DIM/4;
#pragma HLS array_partition variable=a block factor=FACTOR dim=1
#pragma HLS array_partition variable=b block factor=FACTOR dim=2

    // stream in first matrix
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE II=1
            b[i][j] = update_kappa.read();

        }

    // stream in the vector
    for (int i = 0; i < ROW; i++){
#pragma HLS PIPELINE II=1
        a[i] = update_input.read();
    }


    std::complex<float> conj;
    	L1:for(int col = 0; col < COL; ++col){
    		L2:for(int row = 0; row < ROW; ++row){
				#pragma HLS PIPELINE II=1
    			  conj.real(b[row][col].real() * a[row].real() + b[row][col].imag() * a[row].imag());
    			  conj.imag(-b[row][col].real() * a[row].imag() - b[row][col].imag() * a[row].real());
    			  c[row][col] += conj;
    		}

    	}

    // stream out result matrix
    data_struct<complex_float> out_data;

    for (int i = 0; i < ROW; i++)
    {
    	for(int j = 0; j < COL; j++){
#pragma HLS PIPELINE II=1
        out_data.data = c[i][j];
        out_data.last = (i == (ROW - 1) && j == (COL-1)) ? 1 : 0;
        c[i][j] = 0;
        update_output.write(out_data);
    	}

    }

    return;
}
