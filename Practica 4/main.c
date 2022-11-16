#include <stdio.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <math.h>

#define NHILOS 12

typedef struct {
    int numHilo;
    double *chrisRamos;
} argHilo;

void *gestorProcesos(void *argsMessi) {
    argHilo n = *(argHilo *) argsMessi;


    //for (int j = 0; j < 10000000; j+=100*NHILOS) {


        for (int i = 0; i < 100 ; ++i) {
            //*n.chrisRamos += (pow(-1., i) / pow(4., i)) * (2.00 / (4. * i + 1) + 2.0 / (4. * i + 2) + 1 / (4. * i + 3));
            *n.chrisRamos += 1;


        }
    //}


}

int main() {

    int hiloActual;
    pthread_t hilos[NHILOS];
    double valoresHilos[NHILOS];
    argHilo estructuraArgs;


    for (int i = 1; i < NHILOS+1; ++i) {

        estructuraArgs.numHilo = i;
        estructuraArgs.chrisRamos = &valoresHilos[i-1];

        pthread_create(&hilos[i-1],NULL,gestorProcesos,(void*)&estructuraArgs);

    }

    for (int i = 0; i < NHILOS; ++i) {
        pthread_join(hilos[i], NULL);
    }
    double resultado ;
    for (int i = 0; i < NHILOS; ++i) {
      resultado += valoresHilos[i];

    }

    double resultado2;
    for (int i = 0; i < 10000000; ++i) {
        resultado2 += (pow(-1, i) / pow(4.00, i)) * (2.00 / (4 * i + 1) + 2.0 / (4. * i + 2) + 1 / (4. * i + 3));

    }

    printf("\nResultado: %lf\n",resultado2);
    printf("\nResultado: %lf\n",resultado);



    return 0;
}
