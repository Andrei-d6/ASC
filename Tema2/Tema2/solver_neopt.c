/*
 * Tema 2 ASC
 * 2020 Spring
 */
#include "utils.h"

/*
 * Add your unoptimized implementation here
 */
double* my_solver(int N, double *A, double* B) {
	printf("NEOPT SOLVER\n");

	int i, j, k;
	/* C - matricea rezultat */
	double *C = malloc((N * N) * sizeof(double));
	/* A_2 - matrice auxiliara pentru calculul A*A */
	double *A_2 = calloc((N * N), sizeof(double));
	
	/* A_2 = A*A (calculul A^2) */
	for (i = 0; i < N; i++) {
		for (j = i; j < N; j++) {
			for(k = i; k <= j; k++)
				A_2[i*N + j] += A[i*N + k] * A[k*N + j];
		}
	}

	/* C = A_2 * B */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for(k = i; k < N; k++)
				C[i*N + j] += A_2[i*N + k] * B[k*N + j];
		}
	}

	/* C = C + (B* At) */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for(k = j; k < N; k++)
				C[i*N + j] += B[i*N + k] * A[j*N + k];
		}
	}

	return C;
}
