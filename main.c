#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
//#include <wait.h>
#include <math.h>
#include <wait.h>
#include <sys/time.h>

int main() {

    pid_t messi = fork();
    if (messi == -1) {
        fprintf(stdout, " Error: el fork no se ha podido realizar con exito");
    }


    FILE *archivo = fopen("./resultados.log", "a+");
    if (archivo == NULL) {
        fprintf(stdout, "No se ha podido acceder correctamente al archivo.\n");
        exit(-1);
    }

    double x = 0, xmed, contx = 0;//media numeros pares
    double y = 0, ymed, conty = 0;//media numeros impares

    if (messi > 0) {

        pid_t dybala = fork();
        if (dybala == -1) {
            fprintf(stdout, "\nerror en el fork");
            exit(-1);
        }
        if (dybala == 0) {
            struct timeval inicio2;
            struct timeval fin2;

            gettimeofday(&inicio2, NULL);
            //pid_t dybalaPid = getpid();
            printf("\n soy el hijo 2(pares) mi pid es : %d", getpid());
            for (int i = 0; i < 70000000; i += 2) {
                x += tan(sqrt(i));
                contx += 1;
                //printf("x: %f, contx: %f\n",x, contx);
            }
            xmed = x / contx;
            gettimeofday(&fin2, NULL);
            printf("\nCalculos del hijo 2: %f", xmed);
            fprintf(archivo, "h2c: %f\n", xmed);
            printf("Tiempo: %lf\n", (fin2.tv_sec - inicio2.tv_sec + (fin2.tv_usec - inicio2.tv_usec) / 1.e6));
            fprintf(archivo, "h2t: %lf\n", (fin2.tv_sec - inicio2.tv_sec + (fin2.tv_usec - inicio2.tv_usec) / 1.e6));

            // fwrite(archivo,xmed);
            exit(0);
        }
        if (dybala > 0) {
            pid_t nahuelMolina = fork();//
            if (nahuelMolina == -1) {
                fprintf(stderr, "Fallo al realizar el fork");
                exit(-1);

            }
            if (nahuelMolina == 0) {
                struct timeval inicio3;
                struct timeval fin3;

                gettimeofday(&inicio3, NULL);
                double resultadoNahuelMolina = 0;
                for (int i = 0; i < 70000000; i++) {
                    resultadoNahuelMolina += tan(sqrt(i));
                }
                gettimeofday(&fin3, NULL);
                printf("Tiempo: %lf\n", (fin3.tv_sec - inicio3.tv_sec + (fin3.tv_usec - inicio3.tv_usec) / 1.e6));
                fprintf(archivo, "h3t: %lf\n",
                        (fin3.tv_sec - inicio3.tv_sec + (fin3.tv_usec - inicio3.tv_usec) / 1.e6));
                fprintf(archivo, "h3c: %f\n", resultadoNahuelMolina / 70000000);
                exit(0);

            }

        }
        int status;//no necesitamos status pero la declaramos porque la requiere la funcion
        waitpid(dybala, &status, WEXITED);
        waitpid(messi, &status, WEXITED);
        pid_t tagliafico = fork();
        if (tagliafico == -1) {
            fprintf(stdout, "\n error: no se pudo hacer el fork");
            exit(-1);
        }
        if (tagliafico == 0) {
            fgetc(archivo)
        }
        if (tagliafico > 0) {
            printf("\n soy el padre mi pid es : %d", getpid());
            exit(0);
        }
    }

    if (messi == 0) {
        struct timeval inicio;
        struct timeval fin;

        gettimeofday(&inicio, NULL);

        printf("\n soy el hijo 1 (impares) mi pid es : %d", getpid());
        for (int i = 1; i < 70000000; i += 2) {
            y += tan(sqrt(i));
            conty += 1;
        }

        ymed = y / conty;
        gettimeofday(&fin, NULL);
        printf("\nCalculos del hijo 1: %f\n", ymed);
        printf("Tiempo: %lf\n", (fin.tv_sec - inicio.tv_sec + (fin.tv_usec - inicio.tv_usec) / 1.e6));
        fprintf(archivo, "h1t: %lf\n", (fin.tv_sec - inicio.tv_sec + (fin.tv_usec - inicio.tv_usec) / 1.e6));
        fprintf(archivo, "h1c: %f\n", ymed);
        exit(0);
    }


    return 0;
}

// añadir al final del co