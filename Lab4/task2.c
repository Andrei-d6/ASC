#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>     // provides int8_t, uint8_t, int16_t etc.
#include <stdlib.h>
#include <time.h>

struct particle
{
    int8_t v_x, v_y, v_z;
};

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("apelati cu %s <n>\n", argv[0]);
        return -1;
    }

    long n = atol(argv[1]);

    // TODO
    // alocati dinamic o matrice de n x n elemente de tip struct particle
    // verificati daca operatia a reusit
    /*
    struct particle **vect;

    vect = (struct particle **)malloc(n * sizeof(struct particle));
    
    if(vect == NULL) {
        printf("Memory error\n");
        return 12;
    }

    for(long i = 0; i < n; i++) {
        vect[i] = (struct particle *)malloc(n * sizeof(struct particle));
        if(vect[i] == NULL) {
            printf("Memory error\n");
            return 12;
        }
    }
    */

    struct particle* vect = (struct particle*)malloc((n * n) * sizeof(struct particle));
    if(vect == NULL)
        return 12;

    // TODO
    // populati matricea alocata astfel:
    // *liniile pare contin particule cu toate componentele vitezei pozitive
    //   -> folositi modulo 128 pentru a limita rezultatului lui rand()
    // *liniile impare contin particule cu toate componentele vitezi negative
    //   -> folositi modulo 129 pentru a limita rezultatului lui rand()
    
    /*
    for(long i = 0; i < n * n; i++) {
        vect[i].v_x = (uint8_t)rand();
        vect[i].v_y = (uint8_t)rand();
            
        if(i % 2 == 0)
            vect[i].v_z = (uint8_t)rand();
        else
            vect[i].v_z = -(uint8_t)rand();
    }
    */
    time_t t;
    srand((unsigned) time(&t));
    for(long i = 0; i < n * n; i++) {
         if((i % n) % 2 == 0) {
            vect[i].v_x = (uint8_t)rand() % 128;
            vect[i].v_y = (uint8_t)rand() % 128;
            vect[i].v_z = (uint8_t)rand() % 128;
        } else {
            vect[i].v_x = (uint8_t)rand() % 129;
            vect[i].v_y = (uint8_t)rand() % 129;
            vect[i].v_z = -(uint8_t)rand() % 129;
        }
    }

    // TODO
    // scalati vitezele tuturor particulelor cu 0.5
    //   -> folositi un cast la int8_t* pentru a parcurge vitezele fara
    //      a fi nevoie sa accesati individual componentele v_x, v_y, si v_z
    

    /*for(long i = 0; i < n; i++)
        for(long j = 0; j < n; j++){
            vect[i][j].v_x >>= 1;
            vect[i][j].v_y >>= 1;
            vect[i][j].v_z >>= 1;
        }
    */

    //vect = (int8_t *)vect;
    for(long i = 0; i < n * n; i++) {
        vect[i].v_x >>= 1;
        vect[i].v_y >>= 1;
        vect[i].v_z >>= 1;
    }


    // compute max particle speed
    float max_speed = 0.0f;
    
    for(long i = 0; i < n * n; ++i)
    {
        float speed = sqrt(vect[i].v_x * vect[i].v_x +
                           vect[i].v_y * vect[i].v_y +
                           vect[i].v_z * vect[i].v_z);
        if(max_speed < speed) max_speed = speed;
    }
    
    /*
    for(long i = 0; i < n; ++i) {
        for(long j = 0; j < n; j++) {
            float speed = sqrt(vect[i][j].v_x * vect[i][j].v_x +
                               vect[i][j].v_y * vect[i][j].v_y +
                               vect[i][j].v_z * vect[i][j].v_z);
            if(max_speed < speed) max_speed = speed;
        }
    }*/

    // print result
    printf("viteza maxima este: %f\n", max_speed);

    free(vect);

    return 0;
}

