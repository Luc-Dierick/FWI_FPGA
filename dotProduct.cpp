//
// Created by Luc on 09/12/2021.
//

#include "dotProduct.h"

void dotProd(int A[MAGNITUDE][GRIDPOINTS], int B[GRIDPOINTS], int out[MAGNITUDE]) {

	int A_db[MAGNITUDE][GRIDPOINTS];
	int B_db[GRIDPOINTS];

	#pragma HLS array_partition  variable=A_db dim=2 complete
	#pragma HLS array_partition  variable=B_db dim=1 complete
    
	LOOP_A_I:for(int i=0; i<MAGNITUDE; i++){
	    LOOP_A_J:for(int j=0; j <GRIDPOINTS;j++){
            #pragma HLS PIPELINE
	        A_db[i][j] = A[i][j];
	    }
	}

	LOOP_B:for(int i=0; i<GRIDPOINTS; i++){
	    #pragma HLS PIPELINE
		B_db[i] = B[i];
	}

 	LOOP_PROD:for(int i=0;i<MAGNITUDE;i++) {
	    for(int j = 0; j < GRIDPOINTS; j++){
		#pragma HLS PIPELINE
            out[i] += A_db[i][j]*B_db[j];
	    }
	}

}
