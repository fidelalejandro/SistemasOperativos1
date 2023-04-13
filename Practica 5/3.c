//
// Created by alejandro on 18/11/22.
//
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <wait.h>


int main(){
    float messi,messi1,messi2;

    printf("%d\n",getpid());

    int* ptr;	// puntero a enteros //
    int* ptr2;	// otro puntero //

    printf("Dir memoria 1 antes del malloc: %p\n", ptr);
    printf("Dir memoria 2 antes del malloc: %p\n", ptr2);

    // reserva hueco para 300 enteros //
    ptr = (int*)malloc ( 300000*sizeof(int) );
    scanf("%f",&messi);

    //ptr[33] = 15;		// trabaja con el área de memoria //
    printf("Dir memoria 1 después del malloc: %p\n", ptr);
    printf("Dir memoria 1 3000 después del malloc: %p\n", &ptr[2000]);
    printf("Dir memoria 2 después del malloc: %p\n", ptr2);

    ptr2 = ptr;		// asignación a otro puntero //
    printf("Dir memoria 1 después de la asignación: %p\n", ptr);
    printf("Dir memoria 2 después de la asignación: %p\n", ptr2);

    scanf("%f", &messi1);

    free(ptr);
    printf("Dir memoria 1 después del free: %p\n", ptr);
    printf("Dir memoria 2 después del free: %p\n", ptr2);

    scanf("%f", &messi2);

};
//char command[Max_str_len
//sprintf(command
//system(command)
