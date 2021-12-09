//
// Created by Luc on 09/12/2021.
//

#include "dotProduct.h"

void dotProd(double A[N], double B[N], double &out) {

	double A_db[N],B_db[N];

	#pragma HLS array_partition  variable=A_db dim=1 complete
	#pragma HLS array_partition  variable=B_db dim=1 complete

	double product = 0;

	LOOP_A:for(int i=0; i<N; i++){
	#pragma HLS PIPELINE
		A_db[i] = A[i];
	}

	LOOP_B:for(int i=0; i<N; i++){
	#pragma HLS PIPELINE
			B_db[i] = B[i];
	}

	LOOP_PROD:for(int i=0;i<N;i++) {
#pragma HLS UNROLL
		product = product + A_db[i]*B_db[i];
	}

	out =  product;

}
