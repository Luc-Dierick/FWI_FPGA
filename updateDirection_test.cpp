#include "updateDirection.h"
#define SIZE (DIM * DIM)


void updateDirection(hls::stream<complex_float> &resVector, hls::stream<complex_float> &kappa, hls::stream<data_struct<complex_float> > &kappaTimesRes);

void updateDirection_sw(std::complex<float> residualVector[ROW], std::complex<float> kappa[ROW][COL], std::complex<float> kappaTimesResidual[ROW][COL]) {

	std::complex<float> conj;

	for(int col = 0; col < COL; ++col){
		for(int row = 0; row < ROW; ++row){
			  conj.real(kappa[row][col].real() * residualVector[row].real() + kappa[row][col].imag() * residualVector[row].imag());
			  conj.imag(-kappa[row][col].real() * residualVector[row].imag() - kappa[row][col].imag() * residualVector[row].real());
			  kappaTimesResidual[row][col] += conj;
		}

	}
}

int main(void)
{

    int ret_val = 0;

    int i,j, err;

    static complex_float resVect[ROW];
    static complex_float kappa[ROW][COL];
    static complex_float kappaTimesResSW[ROW][COL];
    static complex_float kappaTimesResHW[ROW][COL];


    /** Input Initiation */
    for(i = 0; i<ROW; i++){
        for(j = 0; j<COL; j++){
            kappa[i][j] = {i*1.0f,j*i*0.33f};
        }
    }

    for(i = 0; i<ROW; i++)
        resVect[i] =  {i*1.0f,i*0.33f};
    /** End of Initiation */


    // Streams creation
    hls::stream<complex_float> resVectIn("in");
    hls::stream<complex_float> kappaIn("in2");
    hls::stream<data_struct<complex_float>> kappaTimesResOut("out");

    for(i = 0; i<ROW; i++)
        for(j = 0; j<COL; j++)
            kappaIn.write(kappa[i][j]);

    for(i = 0; i<ROW; i++)
        resVectIn.write(resVect[i]);

    /* HW Matrix Multiplication */
    updateDirection(resVectIn, kappaIn, kappaTimesResOut);

    // Write Output
    for(i = 0; i<ROW; i++)
    	for(j = 0; j < COL; j++)
        kappaTimesResHW[i][j] = kappaTimesResOut.read().data;

    /* reference Matrix Multiplication */
    updateDirection_sw(resVect, kappa, kappaTimesResSW);

    /** Matrix comparison */
    err = 0;
    for (i = 0; i<ROW; i++){
    	for(int j = 0; j < COL; j++){
			if (kappaTimesResSW[i][j] != kappaTimesResHW[i][j]){
				err++;
				std::cout << kappaTimesResSW[i][j] << " HW: " << kappaTimesResHW[i][j] <<std::endl;
			}else{

			}
    	}
    }
                std::cout << kappaTimesResSW[3][3] <<std::endl;
    if (err == 0)
        printf("Matrixes identical ... Test successful!\r\n");
    else
        printf("Test failed! error: %d\r\n",err);

    return err;

}
