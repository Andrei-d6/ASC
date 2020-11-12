#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>


int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("apelati cu %s <step_size> <vector_size> <iterations>\n", argv[0]);
        return -1;
    }

    int64_t l = atoi(argv[1]);  // pasul
    int64_t n = atoi(argv[2]);  // dimensiunea vectorului
    //int64_t c = atoi(argv[3]);  // numarul de iteratii

    // TODO alocari si initializari
    int8_t* a = (int8_t *)malloc(sizeof(int8_t) * n);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // TODO bucla de test
    // in variabila ops calculati numarul de operatii efectuate

    int64_t ops = 0;
    for(int i = 0; i < n; i += 2){
        a[i]++;
        ops++;
    }

    gettimeofday(&end, NULL);

    float elapsed = ((end.tv_sec - start.tv_sec)*1000000.0f + end.tv_usec - start.tv_usec)/1000000.0f;
    printf("%12ld, %12ld, %12f, %12g\n", l, ops, elapsed, elapsed/ops);

    free(a);

    return 0;
}

