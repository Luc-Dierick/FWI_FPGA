#include <iostream>
#include "dotProduct.h"

int main() {
    double A[5];
    double B[5];
    for(int i = 0; i < 5; i++){
        A[i] = i;
        B[i] = i+1;
    }
    double test = dotProd(A,B,5);
    return 0;
}






