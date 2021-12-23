#include "updateDirection.h"

void updateDirection(hls::stream<complex_float> &resV, hls::stream<complex_float> &kappa, hls::stream<data_struct<complex_float> > &s_out) {
#pragma HLS INTERFACE axis port=resV
#pragma HLS INTERFACE axis port=kappa
#pragma HLS INTERFACE axis port=s_out
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
            b[i][j] = kappa.read();

        }

    // stream in the vector
    for (int i = 0; i < ROW; i++){
#pragma HLS PIPELINE II=1
        a[i] = resV.read();
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
        s_out.write(out_data);
    	}

    }

    return;
}
