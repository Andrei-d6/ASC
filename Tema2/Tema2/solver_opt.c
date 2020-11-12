/*
 * Tema 2 ASC
 * 2020 Spring
 */
#include "utils.h"

/*
 * Add your optimized implementation here
 */
double* my_solver(int N, double *A, double* B) {
	printf("OPT SOLVER\n");

	int i, j, k;
	/* C - matricea rezultat */
	double *C = malloc((N * N) * sizeof(double));
	/* A_2 - matrice auxiliara pentru calculul A*A */
	double *A_2 = malloc((N * N) * sizeof(double));

	/* A_2 = A*A (calculul A^2) */
	for (i = 0; i < N; i++) {
		double *orig_pa2 = A_2 + i*N;

		for (k = i; k < N; k++) {
			register double *pa2 = orig_pa2 + i;
			register double *pa = A + k*N + i;
			register double ai = A[i*N + k];

			for(j = i; j < N; j+=10) {
				*pa2 += ai * *pa;
                *(pa2 + 1) += ai * *(pa + 1);
                *(pa2 + 2) += ai * *(pa + 2);
                *(pa2 + 3) += ai * *(pa + 3);
                *(pa2 + 4) += ai * *(pa + 4);
                *(pa2 + 5) += ai * *(pa + 5);
                *(pa2 + 6) += ai * *(pa + 6);
                *(pa2 + 7) += ai * *(pa + 7);
                *(pa2 + 8) += ai * *(pa + 8);
                *(pa2 + 9) += ai * *(pa + 9);

				pa2+= 10;
				pa += 10;
			}
		}
	}

	/* C = A_2 * B */
	for (i = 0; i < N; i++) {
		double *orig_c = C + i*N;

		for (k = i; k < N; k++) {
			register double *pc = orig_c;
			register double *pb = B + k*N;
			register double ai2 = A_2[i*N + k];

			for(j = 0; j < N; j+=10) {
				*pc += ai2 * *pb;
                *(pc + 1) += ai2 * *(pb + 1);
                *(pc + 2) += ai2 * *(pb + 2);
                *(pc + 3) += ai2 * *(pb + 3);
                *(pc + 4) += ai2 * *(pb + 4);
                *(pc + 5) += ai2 * *(pb + 5);
                *(pc + 6) += ai2 * *(pb + 6);
                *(pc + 7) += ai2 * *(pb + 7);
                *(pc + 8) += ai2 * *(pb + 8);
                *(pc + 9) += ai2 * *(pb + 9);

				pc += 10;
				pb += 10;
			}
		}
	}

	/* C = C + (B* At) */
	for (i = 0; i < N; i++) {
		double *orig_pb = B + i*N;

		for (j = 0; j < N; j++) {
			register double *pb = orig_pb + j;
			register double *pa = A + j*N + j;
			register double suma = 0;

			for(k = j; k < N; k+=10) {
				suma += *pb * *pa
                + *(pb + 1) * *(pa + 1)
                + *(pb + 2) * *(pa + 2)
                + *(pb + 3) * *(pa + 3)
                + *(pb + 4) * *(pa + 4)
                + *(pb + 5) * *(pa + 5)
                + *(pb + 6) * *(pa + 6)
                + *(pb + 7) * *(pa + 7)
                + *(pb + 8) * *(pa + 8)
                + *(pb + 9) * *(pa + 9);

				pb += 10;
				pa += 10;
			}
			C[i*N + j] += suma;
		}
	}

	return C;	
}
