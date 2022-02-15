#include "updateDirection.h"

void updateDirection(hls::stream<complex_float> &update_input, hls::stream<complex_float> &update_kappa, hls::stream<data_struct<complex_float> > &update_output) {
#pragma HLS INTERFACE axis port=update_input
#pragma HLS INTERFACE axis port=update_kappa
#pragma HLS INTERFACE axis port=update_output
#pragma HLS INTERFACE ap_ctrl_none port=return

    static complex_float a[ROW];
    static complex_float b[LOW][COL];
    static complex_float d[HIGH][COL];
    static complex_float c[COL];
    int factor = 2;
//#pragma HLS array_partition variable=d cyclic factor=factor dim=1
#pragma HLS array_partition variable=b cyclic factor=factor dim=2
#pragma HLS array_partition variable=a cyclic factor=30 dim=1

#pragma HLS RESOURCE variable=d core=RAM_1P_LUTRAM

    // stream in first matrix
    for (int i = 0; i < LOW; i++)
        for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE II=1
            b[i][j] = update_kappa.read();
        }

    // stream in first matrix
    for (int i = 0; i < HIGH; i++)
        for (int j = 0; j < COL; j++) {
#pragma HLS PIPELINE II=1
            d[i][j] = update_kappa.read();
        }


    // stream in the vector
    for (int i = 0; i < ROW; i++){
#pragma HLS PIPELINE II=1
        a[i] = update_input.read();
    }

//    // stream in the vector
//    for (int i = 0; i < HIGH; i++){
//#pragma HLS PIPELINE II=1
//        d[i] = update_input.read();
//    }



    std::complex<float> conj;
	L1:for(int row = 0; row < LOW; ++row){
		L2:for(int col = 0; col < COL; ++col){
				#pragma HLS PIPELINE II=1
    			  conj.real(b[row][col].real() * a[row].real() + b[row][col].imag() * a[row].imag());
    			  conj.imag(-b[row][col].real() * a[row].imag() - b[row][col].imag() * a[row].real());
    			  c[col] += conj;
		}
    }

	L3:for(int row = 0; row < HIGH; ++row){
		L4:for(int col = 0; col < COL; ++col){
			  conj.real(d[row][col].real() * a[LOW+row].real() + d[row][col].imag() * a[LOW+row].imag());
			  conj.imag(-d[row][col].real() * a[LOW+row].imag() - d[row][col].imag() * a[LOW+row].real());
			  c[col] += conj;
		}
	}

    // stream out result matrix
    data_struct<complex_float> out_data;

    	for(int j = 0; j < COL; j++){
#pragma HLS PIPELINE II=1
        out_data.data = c[j];
        out_data.last = (j == (COL-1)) ? 1 : 0;
        c[j] = 0;
        update_output.write(out_data);


    }

    return;
}
