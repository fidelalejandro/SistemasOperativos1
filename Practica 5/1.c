#include <stdio.h>
#include <unistd.h>

//
// Created by alejandro on 18/11/22.
//

float messi;
char ansu[20];

int main(){
    float neyder;
    char juanpe[11];

    printf("%d\n",getpid());

    printf("%p\n",&messi);
    printf("%p\n",ansu);
    printf("%p\n",&neyder);
    printf("%p\n",juanpe);

    scanf("%f",&messi);

}
