#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
//#include <wait.h>
#include <math.h>

int main() {
    pid_t messi = fork();
    if (messi == -1) {
        fprintf(stdout, " Error: el fork no se ha podido realizar con exito");
    }
    pid_t dybala;
//    pid_t status;

    FILE *archivo = fopen("./resultados.log", "a+");
    if (archivo == NULL) {
        fprintf(stdout, "No se ha podido acceder correctamente al archivo.\n");
        exit(-1);
    }

    double x = 0, xmed, contx = 0;//media numeros pares
    double y = 0, ymed, conty = 0;//media numeros impares

    if (messi > 0) {
        dybala = fork();
        if (dybala == 0) {
            printf("\n soy el hijo 2(pares) mi pid es : %d", getpid());
            for (int i = 0; i < 70000000; i += 2) {
                x += tan(sqrt(i));
                contx += 1;
                //printf("x: %f, contx: %f\n",x, contx);
            }
            xmed = x / contx;
            printf("\n %f", xmed);
            //fwrite("Calculos del hijo 1: ",1,sizeof("Calculos del hijo 1: "),archivo);
            fprintf(archivo, "Calculos del hijo 2: %f\n", xmed);

            // fwrite(archivo,xmed);
            exit(0);
        }
        if (dybala > 0) {
            pid_t nahuelMolina = fork();//
            if(nahuelMolina==-1)
            {
                fprintf(stderr,"Fallo al realizar el fork");
            }
            if (nahuelMolina == 0)
            {
                double resultadoNahuelMolina = 0;
                for (int i = 0; i < 70000000; i++) {
                    resultadoNahuelMolina += tan(sqrt(i));
                }
                fprintf(archivo, "Calculos del hijo 3: %f\n", resultadoNahuelMolina / 70000000);
            }
        }
        //wait(dybala);
        printf("\n soy el padre mi pid es : %d", getpid());
        exit(0);

    }

    if (messi == 0) {
        printf("\n soy el hijo 1 (impares) mi pid es : %d", getpid());
        for (int i = 1; i < 70000000; i += 2) {
            y += tan(sqrt(i));
            conty += 1;
        }
        ymed = y / conty;
        printf("\n %f", ymed);
        fprintf(archivo, "Calculos del hijo 1: %f\n", ymed);


    }

    return 0;
}

// añadir al final del co