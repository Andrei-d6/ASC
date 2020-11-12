#include <stdio.h>
#include <stdlib.h>

/*
 *  ////////////////////////////////////////////////////////
 *  ////////////////////////////////////////////////////////
 *  //													  //
 *	//  Optimizarea constantelor si accesului la vectori  //
 *  //													  //
 *  ////////////////////////////////////////////////////////
 *  ////////////////////////////////////////////////////////
 */


//Initializarea dimensiunii matricelor (N*N)
#define N 10

//Afisarea unei matrici liniarizate sub forma matriceala
void printMatrix(double *matrix)
{	
	printf("\n");

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			printf("%d ", (int)matrix[i*N + j]);
		printf("\n");
	}

	printf("\n");
}

int main(int argc, char **argv)
{
	int i, j, k;

	/*
	 * (0, 1, 2)
	 * (3, 4, 5)  => (0, 1, 2, 3, 4, 5, 6, 7, 8) 
	 * (6, 7, 8)
	 */

	double *a = malloc((N * N) * sizeof(double));
	double *b = malloc((N * N) * sizeof(double));
	double *c = malloc((N * N) * sizeof(double));
	
	//Initializarea matricelor liniarizate	
	for (i = 0; i < N * N; i++) {
		a[i] = 1;
		b[i] = 2;
	}

	//Inmultirea matricelor
	for (i = 0; i < N; i++) {
		double *orig_pa = a + i*N;

		for (j = 0; j < N; j++) {
			double *pa = orig_pa;
			double *pb = b + j;
			register double suma = 0;

			for (k = 0; k < N; k++) {
				suma += *pa * *pb;
				pa++;
				pb += N;
			}

			c[(i*N) + j] = suma;
		}
	}

	//Afisarea rezultatului final
	printMatrix(c);

	//Eliberarea memoriei folosite
	free(a);
	free(b);
	free(c);

	return 0;
}