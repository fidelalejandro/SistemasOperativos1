//
// Created by alejandro on 18/11/22.
//

#include <stdio.h>
#include <unistd.h>


float f1(int x){
    int local1=1;
    printf("parametro 1 %p\n",&x);
    printf("variable 1 %p\n",&local1);

}
float f2(int y){
    int local2=2;
    printf("parametro 2 %p\n",&y);
    printf("variable 2 %p\n",&local2);
};

int main(){
    printf("%d\n",getpid());
    float esp = 0;
    printf("%p\n",f1(11));
    printf("%p\n",f2(22));
    scanf("%f",&esp);
};
