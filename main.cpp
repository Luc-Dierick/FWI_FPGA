#include <iostream>
#include "dotProduct.h"

int main() {

    std::complex<double> A[MAGNITUDE][GRIDPOINTS];
    double B[GRIDPOINTS];
    std::complex<double> out[MAGNITUDE];

    for (int i = 0; i < GRIDPOINTS; i++) {
        B[i] = i;
        for (int j = 0; j < MAGNITUDE; j++) {
            A[j][i] = {i*1.0f, j*1.0f};
        }
    }

    for (std::complex<double> & i : out) {
        i = {0,0};
    }

    dotProd(A, B, out);
//    std::cout << "A[0][1]" << A[0][1] <<std::endl;
    std::cout << "OUT[1] is: " << out[1] << std::endl;

    std::complex<double> test = {285,45};
    if (out[1] == test) {
        std::cout << "Test passed! product is: " << out[1] << std::endl;
    } else {
        std::cout << "Test Failed!! product is: " << out[1] << "and should be: 285" << std::endl;
    }

    return 0;
}






