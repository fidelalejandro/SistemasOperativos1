//
// Created by alejandro on 25/11/22.
//
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

int enteroglob = 20;

void *define_actualiza(void *local){
    float espera;
    int nueva;

    printf("\n\nDireccion durante el hilo de la var local: %p\n",&local);
    int loc = (int) local;

    printf("Direccion var global en el hilo: %p\n",&enteroglob);

    printf("Direccion var local sin actualizar en el hilo: %p\n",&nueva);
    scanf("%f\n",&espera);

    nueva = loc * enteroglob;
    printf("Direccion var local actualizada en el hilo: %p\n",&nueva);
    scanf("%f\n",&espera);
}


int main(){
    int enteroloc = 2;
    printf("PID: %i\n",getpid());
    pthread_t hilo1,hilo2;
    float espera;

    printf("Direccion hilo 1 antes de crear hilo: %p\n",&hilo1);
    printf("Direccion hilo 2 antes de crear hilo: %p\n",&hilo2);
    printf("Direccion var local antes de crear hilo: %p\n",&enteroloc);
    printf("Direccion var global antes de crear hilo: %p\n",&enteroglob);

    scanf("%f", &espera);
    pthread_create(&hilo1, NULL, define_actualiza, (void *) enteroloc);
    pthread_create(&hilo2, NULL, define_actualiza, (void *) enteroloc);

    printf("Direccion hilo 1 despues de crear hilo: %p\n",&hilo1);
    printf("Direccion hilo 2 despues de crear hilo: %p\n",&hilo2);
    printf("Direccion var local despues de crear hilo: %p\n",&enteroloc);
    printf("Direccion var global despues de crear hilo: %p\n",&enteroglob);

    scanf("%f", &espera);

    pthread_join(hilo1,NULL);

    pthread_join(hilo2,NULL);

    printf("Direccion hilo 1 despues del join : %p\n",&hilo1);
    printf("Direccion hilo 2 después del join : %p\n",&hilo2);
    printf("Direccion var local despues de join: %p\n",&enteroloc);
    printf("Direccion var global despues de join: %p\n",&enteroglob);


    scanf("%f", &espera);
}