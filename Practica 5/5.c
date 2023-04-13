//
// Created by alejandro on 25/11/22.
//
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <math.h>

int main(){
    printf("%i",getpid());
    float af,op;
    scanf("%f\n",&af);
    printf("%p\n",&op);
    op = cos(5);
    printf("%p\n",&op);
    printf("%f\n",op);
    scanf("%f\n",&af);

}