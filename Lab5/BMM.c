#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
 
void BMMultiply(int n, double** a, double** b, double** c)
{
    int bi=0;
    int bj=0;
    int bk=0;
    int i=0;
    int j=0;
    int k=0;
    // TODO: set block dimension blockSize
    int blockSize=100; 
 
    for(bi=0; bi<n; bi+=blockSize)
        for(bj=0; bj<n; bj+=blockSize)
            for(bk=0; bk<n; bk+=blockSize)
                for(i=0; i<blockSize; i++)
                    for(j=0; j<blockSize; j++)
                        for(k=0; k<blockSize; k++)
                            c[bi+i][bj+j] += a[bi+i][bk+k]*b[bk+k][bj+j];
}

//Afisarea unei matrici liniarizate sub forma matriceala
void printMatrix(int n, double *matrix)
{   
    printf("\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", (int)matrix[i*n + j]);
        printf("\n");
    }

    printf("\n");
}

void BLMMultiply(int n, double *a, double *b, double *c)
{
    int i, j, k;
    int i1, j1, k1;

    //block size
    int B = 100; 

    for (i = 0; i < n; i += B)
        for (k = 0; k < n; k += B)
            for (j = 0; j < n; j += B)
            
                //B * B block multiplication
                for (i1 = i; i1 < i+B; i1++)
                    for (j1 = j; j1 < j+B; j1++)
                        for (k1 = k; k1 < k+B; k1++)
                            c[i1*n + j1] += a[i1*n + k1] * b[k1*n + j1];

}

int main(void)
{
    int n;
    double** A;
    double** B;
    double** C;
    int numreps = 10;
    int i=0;
    int j=0;
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed_normal = 0;
    double elapsed_liniar = 0;

    double *a;
    double *b;
    double *c;
    // TODO: set matrix dimension n
    n = 500;
    // allocate memory for the matrices
    a = calloc((n * n), sizeof(double));
    b = calloc((n * n), sizeof(double));
    c = calloc((n * n), sizeof(double));

    // TODO: allocate matrices A, B & C
    ///////////////////// Matrix A //////////////////////////
    A = (double **)malloc(n * sizeof(double *));

    ///////////////////// Matrix B ////////////////////////// 
    B = (double **)malloc(n * sizeof(double *));

    ///////////////////// Matrix C //////////////////////////
    C = (double **)malloc(n * sizeof(double *));

    for (i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        B[i] = (double *)malloc(n * sizeof(double));
        C[i] = (double *)calloc(n, sizeof(double));
    }

    // Initialize matrices A & B
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            A[i][j] = 1;
            B[i][j] = 2;

            a[i*n +j] = 1;
            b[i*n +j] = 2;
        }
    }
 
    //multiply matrices
 
    printf("Multiply matrices %d times...\n", numreps);
    for (i=0; i<numreps; i++)
    {
        gettimeofday(&tv1, &tz);
        BMMultiply(n,A,B,C);
        gettimeofday(&tv2, &tz);
        elapsed_normal += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    }
    printf("Time = %lf \n\n",elapsed_normal);
    

    printf("Multiply liniarized matrices %d times...\n", numreps);
    for (i=0; i<numreps; i++)
    {
        gettimeofday(&tv1, &tz);
        BLMMultiply(n, a, b, c);
        gettimeofday(&tv2, &tz);
        elapsed_liniar += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
    }
    printf("Time = %lf \n\n",elapsed_liniar);


    printf("Speed-up: %f%c\n", (elapsed_liniar / (elapsed_normal + elapsed_liniar)) * 100.0f, '%');
    //deallocate memory for matrices A, B & C
    for (i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);
    
    free(a);
    free(b);
    free(c);
    return 0;
}