//
// Created by Luc on 09/12/2021.
//
#include "dotProduct.h"
#include <iostream>

using namespace std;

int main(){

    double A[N];
    double B[N];
    for(int i = 0; i < N; i++){
        A[i] = i;
        B[i] = i+1;
    }

    double out;
    dotProd(A,B, out);

    if(out==333300){
    	std::cout << "Test passed! product is: " << out << std::endl;
    }else{
    	std::cout << "Test Failed!! product is: " << out  << std::endl;
    }

    return 0;
}
