
#include "updateDirection.h"
//#include <chrono>
//#include <Windows.h>

void updateDirection(hls::stream<complex_float> &resVector, hls::stream<complex_float> &kappa, hls::stream<data_struct<complex_float> > &kappaTimesRes);

//
//double get_wall_time(){
//    LARGE_INTEGER time,freq;
//    if (!QueryPerformanceFrequency(&freq)){
//        //  Handle error
//        return 0;
//    }
//    if (!QueryPerformanceCounter(&time)){
//        //  Handle error
//        return 0;
//    }
//    return (double)time.QuadPart / freq.QuadPart;
//}
//double get_cpu_time(){
//    FILETIME a,b,c,d;
//    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
//        //  Returns total user time.
//        //  Can be tweaked to include kernel times as well.
//        return
//            (double)(d.dwLowDateTime |
//            ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
//    }else{
//        //  Handle error
//        return 0;
//    }
//}


void updateDirection_sw(std::complex<float> residualVector[ROW], std::complex<float> kappa[ROW][COL], std::complex<float> kappaTimesResidual[ROW][COL]) {

	std::complex<float> conj;

		for(int row = 0; row < ROW; ++row){
			for(int col = 0; col < COL; ++col){
			  conj.real(kappa[row][col].real() * residualVector[col].real() + kappa[row][col].imag() * residualVector[col].imag());
			  conj.imag(-kappa[row][col].real() * residualVector[col].imag() - kappa[row][col].imag() * residualVector[col].real());
			  kappaTimesResidual[row][col] += conj;
		}

	}
}

int main(void)
{

    int ret_val = 0;

    int i,j, err;

    static complex_float resVect[COL];
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

    for(i = 0; i<COL; i++)
        resVectIn.write(resVect[i]);

    /* HW Matrix Multiplication */
    updateDirection(resVectIn, kappaIn, kappaTimesResOut);

    // Write Output
    for(i = 0; i<ROW; i++)
    	for(j = 0; j < COL; j++)
        kappaTimesResHW[i][j] = kappaTimesResOut.read().data;

    /* reference Matrix Multiplication */
//    auto begin = std::chrono::high_resolution_clock::now();
//    double wall0 = get_wall_time();
    updateDirection_sw(resVect, kappa, kappaTimesResSW);
//    double wall1 = get_wall_time();
//
//    auto end = std::chrono::high_resolution_clock::now();
//    printf("Time measured: %.7f wall sec.\n", wall1-wall0);
//    auto elapsed =  std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//    printf("Time measured: %.7f seconds.\n", elapsed.count() * 1e-9);

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
                std::cout << kappaTimesResHW[3][3] <<std::endl;
    if (err == 0)
        printf("Matrixes identical ... Test successful!\r\n");
    else
        printf("Test failed! error: %d\r\n",err);

    return err;

}
