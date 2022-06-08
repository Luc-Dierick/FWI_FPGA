#include "ForwardFunction.h"

void ForwardFunction(hls::stream<complex_float> &in, hls::stream<reg_float> &in2, hls::stream<data_struct<dout_t> > &out);

void mmult_sw(std::complex<float> a[ROW/2][COL], float b[COL], std::complex<float> out[COL]) {
    // dot product of vector and matrix A*B
	for (int col = 0; col < COL; ++col){
		for (int row = 0; row < ROW; ++row){
            out[row] += a[row][col] * b[col];
        }
	}
}

int main(void)
{

    int ret_val = 0;

    int i,j, err;

    static complex_float matOp1[ROW][COL];
    float matOp2[COL];
    static complex_float matMult_sw[ROW];
    static complex_float matMult_hw[ROW];

    /** Matrix Initiation */
    for(i = 0; i<ROW; i++){
        for(j = 0; j<COL; j++){
            matOp1[i][j] = {i*1.0f,j*i*0.33f};
        }
    }



    for(i = 0; i<COL; i++)
        matOp2[i] = (float)(i);
    /** End of Initiation */


    // Streams creation
    hls::stream<complex_float> in("in");
    hls::stream<reg_float> in2("in2");
    hls::stream<data_struct<dout_t>> out("out");


    for(j = 0; j<COL; j++)
    	in2.write(matOp2[j]);
    for(i = 0; i<ROW; i++){
        for(j = 0; j<COL; j++){
        	in.write(matOp1[i][j]);
    }

}

    ForwardFunction(in,in2,out);

    // Write Output
    for(i = 0; i<ROW; i++)
        matMult_hw[i] = out.read().data;



    /* reference Matrix Multiplication */
    mmult_sw(matOp1, matOp2, matMult_sw);



    /** Matrix comparison */
    err = 0;
    for (i = 0; (i<ROW && !err); i++)
        if (matMult_sw[i] != matMult_hw[i]){
            err++;
            std::cout << matMult_sw[i] << matMult_hw[i] <<std::endl;
        }else{

        }
    std::cout << matMult_hw[3]  << std::endl;
    if (err == 0)
        printf("Matrixes identical ... Test successful!\r\n");
    else
        printf("Test failed! error: %d\r\n",err);

    return err;

}
