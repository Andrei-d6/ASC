/*
 * Tema 2 ASC
 * 2020 Spring
 */
#include "utils.h"
#include "cblas.h"
#include <string.h>

/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B) {
	printf("BLAS SOLVER\n");

	int i, j;
	/* C - matricea rezultat */
	double *C = malloc((N * N) * sizeof(double));
	/* A_2 - matrice auxiliara pentru calculul A*A */
	double *A_2 = malloc((N * N) * sizeof(double));
	/* A_2B - matrice auxiliara pentru calculul A*A*B */
	double *A_2B = malloc((N * N) * sizeof(double));

	/* A_2 = A */
	memcpy(A_2, A, N*N*sizeof(double));
	
	/* A_2B = B */
	memcpy(A_2B, B, N*N*sizeof(double));

	/* A_2 = A*A */
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1.0, A, N, A_2, N);

	/* A_2B = A_2 * B */
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1.0, A_2, N, A_2B, N);

	/* B = B*At */
	cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasTrans, CblasNonUnit, N, N, 1.0, A, N, B, N);
		
	/* C = B*At + A_2*B
	 *      B     A_2B
	 * calculul sumei finale (rezultatul final)
	 */
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			C[i*N + j] = B[i*N + j] + A_2B[i*N + j];

	return C;
}

