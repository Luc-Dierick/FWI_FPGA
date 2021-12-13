#include <iostream>
#include "dotProduct.h"

int main() {

    int A[MAGNITUDE][GRIDPOINTS];
    int B[GRIDPOINTS];
    int out[MAGNITUDE];

    for (int i = 0; i < GRIDPOINTS; i++) {
        B[i] = i;
        for (int j = 0; j < MAGNITUDE; j++) {
            A[j][i] = i*j;
        }
    }

    for (int & i : out) {
        i = 0;
    }

    dotProd(A, B, out);
//    std::cout << "A[0][1]" << A[0][1] <<std::endl;
    std::cout << "OUT[1] is: " << out[1] << std::endl;

    if (out[1] == 285) {
        std::cout << "Test passed! product is: " << out[1] << std::endl;
    } else {
        std::cout << "Test Failed!! product is: " << out[1] << "and should be: 285" << std::endl;
    }

    return 0;
}






