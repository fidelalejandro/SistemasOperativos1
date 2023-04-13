//
// Created by alejandro on 25/11/22.
//
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(int argc,char *argv[]){

    float x;

    pid_t padre = getpid();
    pid_t hijo;
    int* whalley;
    printf("Padre: %i\n", padre);

    scanf("%f",&x);

    char *const parmList[] = {"prog",  NULL};


    hijo = fork();
    printf("Hijo: %i\n", hijo);
    if (hijo == -1)
    {
        perror("Error: el fork no se ha podido realizar con exito");
        exit(-1);
    }
    if (hijo == 0){
        printf("Hijo creado satisfactoriamente\n");
        whalley = (int*)malloc ( 300000*sizeof(int) );
        printf("%p\n",whalley);
        scanf("%f",&x);
        execv("./prog", parmList);
        printf("Return not expected. Must be an execv error.n");
        scanf("%f",&x);

    }

    scanf("%f",&x);

}
//la funcion malloc reserva por bloques de memoria