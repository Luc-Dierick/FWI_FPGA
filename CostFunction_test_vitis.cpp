
#include "CostFunction_vitis.h"

void updateDirection(hls::stream<complex_float> &resVector, hls::stream<complex_float> &kappa, hls::stream<complex_float> &kappaTimesRes);


void updateDirection_sw(complex_float residualVector[ROW], complex_float kappa[ROW][COL], complex_float kappaTimesResidual[COL]) {

    complex_float conj;

    for(int row = 0; row < ROW; ++row){
        for(int col = 0; col < COL; ++col){
            conj.data.real(kappa[row][col].data.real() * residualVector[row].data.real() + kappa[row][col].data.imag() * residualVector[row].data.imag());
            conj.data.imag(-kappa[row][col].data.real() * residualVector[row].data.imag() - kappa[row][col].data.imag() * residualVector[row].data.real());
            kappaTimesResidual[col].data += conj.data;
        }

    }
}

int main(void)
{

    int ret_val = 0;

    int i,j, err;

    static complex_float resVect[ROW];
    static complex_float kappa[ROW][COL];
    static complex_float kappaTimesResSW[COL];
    static complex_float kappaTimesResHW[COL];


    /** Input Initiation */
    for(i = 0; i<ROW; i++){
        for(j = 0; j<COL; j++){
            kappa[i][j].data = {i*1.0f,j*i*0.33f};
        }
    }

    for(i = 0; i<ROW; i++)
        resVect[i].data =  {i*1.0f,i*0.33f};
    /** End of Initiation */


    // Streams creation
    hls::stream<complex_float> resVectIn("in");
    hls::stream<complex_float> kappaIn("in2");
    hls::stream<complex_float> kappaTimesResOut("out");

    for(i = 0; i<ROW; i++)
        for(j = 0; j<COL; j++)
            kappaIn.write(kappa[i][j]);

    for(i = 0; i<ROW; i++)
        resVectIn.write(resVect[i]);

    /* HW Matrix Multiplication */
    updateDirection(resVectIn, kappaIn, kappaTimesResOut);

    // Write Output
    for(j = 0; j < COL; j++)
        kappaTimesResHW[j] = kappaTimesResOut.read();


    updateDirection_sw(resVect, kappa, kappaTimesResSW);

    /** Matrix comparison */
    err = 0;
        for(int j = 0; j < COL; j++){
            if (kappaTimesResSW[j].data != kappaTimesResHW[j].data){
                err++;
                std::cout << kappaTimesResSW[j].data << " HW: " << kappaTimesResHW[j].data <<std::endl;
            }else{

            }
        }

    std::cout << kappaTimesResHW[3].data <<std::endl;
    if (err == 0)
        printf("Matrixes identical ... Test successful!\r\n");
    else
        printf("Test failed! error: %d\r\n",err);

   std::cout << kappaTimesResHW[7].data <<std::endl;
    return err;

}

