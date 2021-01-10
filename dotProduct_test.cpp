#include "dotProduct.h"

void dotprod(hls::stream<din_t> &in, hls::stream<din2_t> &in2, hls::stream<data_struct<dout_t> > &out);

void mmult_sw(std::complex<float> a[DIM][DIM], float b[DIM], std::complex<float> out[DIM]) {
    // dot product of vector and matrix A*B
    for (int ia = 0; ia < DIM; ++ia)
        for (int ib = 0; ib < DIM; ++ib) {
            out[ia] += a[ia][ib] * b[ib];
        }
}

int main(void)
{

    int ret_val = 0;

    int i,j, err;

    static din_t matOp1[DIM][DIM];
    float matOp2[DIM];
    static din_t matMult_sw[DIM];
    static din_t matMult_hw[DIM];

    /** Matrix Initiation */
    for(i = 0; i<DIM; i++){
        for(j = 0; j<DIM; j++){
            matOp1[i][j] = {i*1.0f,j*i*0.33f};
        }
    }

    for(i = 0; i<DIM; i++)
        matOp2[i] = (float)(i);
    /** End of Initiation */


    // Streams creation
    hls::stream<din_t> in("in");
    hls::stream<din2_t> in2("in2");
    hls::stream<data_struct<dout_t>> out("out");

    for(i = 0; i<DIM; i++)
        for(j = 0; j<DIM; j++)
            in.write(matOp1[i][j]);

    for(i = 0; i<DIM; i++)
        in2.write(matOp2[i]);

    /* HW Matrix Multiplication */
    dotprod(in, in2, out);

    // Write Output
    for(i = 0; i<DIM; i++)
        matMult_hw[i] = out.read().data;

    /* reference Matrix Multiplication */
    mmult_sw(matOp1, matOp2, matMult_sw);

    /** Matrix comparison */
    err = 0;
    for (i = 0; (i<DIM && !err); i++)
        if (matMult_sw[i] != matMult_hw[i]){
            err++;
        }else{
//            std::cout << matOp1[i][i] <<std::endl;
        }
    std::cout << matMult_hw[3]  << std::endl;
    if (err == 0)
        printf("Matrixes identical ... Test successful!\r\n");
    else
        printf("Test failed! error: %d\r\n",err);

    return err;

}
