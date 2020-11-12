#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 

/*
 *  ////////////////////////////////////////////////////////
 *  ////////////////////////////////////////////////////////
 *  //													  //
 *	//  Ordonarea buclelor folosind matrici liniarizate   //
 *  //													  //
 *  ////////////////////////////////////////////////////////
 *  ////////////////////////////////////////////////////////
 */

//Initializarea dimensiunii matricelor (N*N)
#define N 800

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

//i-j-k: 	Constant 	Secvential 	Nesecvential 
double ijkSimple(double *a, double *b, double *c)
{
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for(k = 0; k < N; k++)
				c[i*N + j] += a[i*N + k] * b[k*N + j];
		}
	}

    gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

	return elapsed;
}

double ijkOptimized(double *a, double *b, double *c)
{	
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

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

	gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

	return elapsed;
}

//i-k-j: 	Secvential 	Constant 	Secvential
double ikj(double *a, double *b, double *c)
{
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

	for (i = 0; i < N; i++) {
		for (k = 0; k < N; k++) {
			for(j = 0; j < N; j++)
				c[i*N + j] += a[i*N + k] * b[k*N + j];
		}
	}

    gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    //printMatrix(c);
	return elapsed;
}

//j-i-k: 	Constant 	Secvential 	Nesecvential 
double jik(double *a, double *b, double *c)
{
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

	for (j = 0; j < N; j++) {
		for (i = 0; i < N; i++) {
			for(k = 0; k < N; k++)
				c[i*N + j] += a[i*N + k] * b[k*N + j];
		}
	}

    gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
	//printMatrix(c);
	return elapsed;
}

//j-k-i: 	Nesecvential 	Nesecvential 	Constant 
double jki(double *a, double *b, double *c)
{
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

	for (j = 0; j < N; j++) {
		for (k = 0; k < N; k++) {
			for(i = 0; i < N; i++)
				c[i*N + j] += a[i*N + k] * b[k*N + j];
		}
	}

    gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    //printMatrix(c);
	return elapsed;
}

//k-i-j: 	Secvential 	Constant 	Secvential 
double kij(double *a, double *b, double *c)
{
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

	for (k = 0; k < N; k++) {
		for (i = 0; i < N; i++) {
			for(j = 0; j < N; j++)
				c[i*N + j] += a[i*N + k] * b[k*N + j];
		}
	}

    gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    //printMatrix(c);
	return elapsed;
}

//k-j-i: 	Nesecvential 	Nesecvential 	Constant 
double kji(double *a, double *b, double *c)
{
	int i, j, k;
	struct timeval tv1, tv2;
    struct timezone tz;
	double elapsed = 0;

	gettimeofday(&tv1, &tz);

	for (k = 0; k < N; k++) {
		for (j = 0; j < N; j++) {
			for(i = 0; i < N; i++)
				c[i*N + j] += a[i*N + k] * b[k*N + j];
		}
	}

    gettimeofday(&tv2, &tz);
    elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    //printMatrix(c);
	return elapsed;
}

int main(int argc, char **argv)
{
    double elapsed;

    double *a = calloc((N * N), sizeof(double));
	double *b = calloc((N * N), sizeof(double));
	double *c = calloc((N * N), sizeof(double));
	
	//Initializarea matricelor liniarizate	
	for (int i = 0; i < N * N; i++) {
		a[i] = 1;
		b[i] = 2;
	}

	printf("Starting test for i-j-k (Simple)\n");
    elapsed = ijkSimple(a, b, c);
    printf("Time i-j-k Simple = %lf \n\n",elapsed);

    for (int i = 0; i < N * N; i++)
    	c[i] = 0;

    printf("Starting test for i-j-k (Optimized)\n");
    elapsed = ijkOptimized(a, b, c);
    printf("Time i-j-kOptimized = %lf \n\n",elapsed);

    for (int i = 0; i < N * N; i++)
    	c[i] = 0;

    printf("Starting test for i-k-j\n");
    elapsed = ikj(a, b, c);
    printf("Time i-k-j = %lf \n\n",elapsed);

    for (int i = 0; i < N * N; i++)
    	c[i] = 0;

    printf("Starting test for j-i-k\n");
    elapsed = jik(a, b, c);
    printf("Time j-i-k = %lf \n\n",elapsed);

    for (int i = 0; i < N * N; i++)
    	c[i] = 0;

    printf("Starting test for j-k-i\n");
    elapsed = jki(a, b, c);
    printf("Time j-k-i = %lf \n\n",elapsed);

    for (int i = 0; i < N * N; i++)
    	c[i] = 0;

    printf("Starting test for k-i-j\n");
    elapsed = kij(a, b, c);
    printf("Time k-i-j = %lf \n\n",elapsed);

    for (int i = 0; i < N * N; i++)
    	c[i] = 0;

    printf("Starting test for k-j-i\n");
    elapsed = kji(a, b, c);
    printf("Time k-j-i = %lf \n\n",elapsed);

    //Eliberarea memoriei folosite
	free(a);
	free(b);
	free(c);

    return 0;
}