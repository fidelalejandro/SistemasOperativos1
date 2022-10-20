#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wait.h>
#include <sys/time.h>

#define K_70_MILLONES  70000000

void calculaMediasTangentes(FILE *archivo, int numHijo) {

    int valorInicio, paso;

    switch (numHijo) {
        case 1:
            printf("\nSoy el hijo 1 (pares) mi pid es: %d\n", getpid());
            valorInicio = 0;
            paso = 2;
            break;

        case 2:
            printf("\nSoy el hijo 2 (impares) mi pid es: %d\n", getpid());
            valorInicio = 1;
            paso = 2;
            break;
        case 3:
            printf("\nSoy el hijo 3 (TODOS) mi pid es: %d\n", getpid());
            valorInicio = 0;
            paso = 1;
            break;
        default:
            fprintf(stdout, "Error\n");
            return;
    }


    struct timeval tv_overhead;
    struct timeval tv_inicio;
    struct timeval tv_fin;

    gettimeofday(&tv_overhead, NULL);
    gettimeofday(&tv_inicio, NULL);


    double sumaTangentes = 0, sumasRealizadas = 0;

    for (int i = valorInicio; i < K_70_MILLONES; i += paso) {
        sumaTangentes += tan(sqrt(i));
        sumasRealizadas += 1;
        //printf("sumaTangentes: %f, sumasRealizadas: %f\n",sumaTangentes, sumasRealizadas);
    }

    double media = sumaTangentes / sumasRealizadas;

    gettimeofday(&tv_fin, NULL);

    double tiempoOverhead = (tv_inicio.tv_sec - tv_overhead.tv_sec + (tv_inicio.tv_usec - tv_overhead.tv_usec) / 1.e6);
    double tiempoCalculo = (tv_fin.tv_sec - tv_inicio.tv_sec + (tv_fin.tv_usec - tv_inicio.tv_usec) / 1.e6);
    tiempoCalculo -= tiempoOverhead;

    printf("Media calculada por el hijo %d: %lf\n", numHijo, media);
    fprintf(archivo, "Hijo %d: media: %lf\n", numHijo, media);
    printf("Tiempo utilizado por el hijo %d: %lf\n", numHijo, tiempoCalculo);
    fprintf(archivo, "Hijo %d: tiempo: %lf\n", numHijo, tiempoCalculo);
}


void calculaMediaDesdeArchivo(FILE *archivo) {

    printf("\nSoy el hijo 4 mi pid es: %d\n", getpid());

    struct timeval tv_overhead;
    struct timeval tv_inicio;
    struct timeval tv_fin;

    gettimeofday(&tv_overhead, NULL);
    gettimeofday(&tv_inicio, NULL);

    /* char control = '\0';
    int cont = 0;
    double uno, dos;*/
    fseek(archivo, 0, SEEK_SET);

    char *linea = NULL;
    size_t len = 0;
    ssize_t read;

    double media = 0;
    int cuenta = 0;
    while ((cuenta < 2) && ((read = getline(&linea, &len, archivo)) != -1)) {

        //printf("Linea recuperada de longitud %zu: %s\n", read, linea);
        if (strncmp(linea, "Hijo 1: media:", 14) == 0 || strncmp(linea, "Hijo 2: media:", 14) == 0) {
            double valor;
            int ok = sscanf(linea + 14, "%lf", &valor);
            if (ok) {
                media += valor;
                cuenta++;
            }
        }
    }

    if (linea) {
        free(linea);
    }


    gettimeofday(&tv_fin, NULL);

    double tiempoOverhead = (tv_inicio.tv_sec - tv_overhead.tv_sec + (tv_inicio.tv_usec - tv_overhead.tv_usec) / 1.e6);
    double tiempoCalculo = (tv_fin.tv_sec - tv_inicio.tv_sec + (tv_fin.tv_usec - tv_inicio.tv_usec) / 1.e6);
    tiempoCalculo -= tiempoOverhead;


    if (cuenta == 2) {
        media /= cuenta;

        printf("Media calculada por el hijo %d: %lf\n", 4, media);
        fprintf(archivo, "Hijo %d: media: %lf\n", 4, media);
        printf("Tiempo utilizado por el hijo %d: %lf\n", 4, tiempoCalculo);
        fprintf(archivo, "Hijo %d: tiempo: %lf\n", 4, tiempoCalculo);
    } else {
        fprintf(stdout, "ERROR: No se han encontrado los datos a procesar.\n");
    }
}

void calculaDiferenciaDesdeArchivo(FILE *archivo) {

    printf("\nSoy el hijo 4 mi pid es: %d\n", getpid());

    struct timeval tv_overhead;
    struct timeval tv_inicio;
    struct timeval tv_fin;

    gettimeofday(&tv_overhead, NULL);
    gettimeofday(&tv_inicio, NULL);

    /* char control = '\0';
    int cont = 0;
    double uno, dos;*/
    fseek(archivo, 0, SEEK_SET);

    char *linea = NULL;
    size_t len = 0;
    ssize_t read;

    double media = 0;
    int cuenta = 0;
    while ((cuenta < 2) && ((read = getline(&linea, &len, archivo)) != -1)) {

        //printf("Linea recuperada de longitud %zu: %s\n", read, linea);
        if (strncmp(linea, "Hijo 1: media:", 14) == 0 || strncmp(linea, "Hijo 2: media:", 14) == 0) {
            double valor;
            int ok = sscanf(linea + 14, "%lf", &valor);
            if (ok) {
                media += valor;
                cuenta++;
            }
        }
    }

    if (linea) {
        free(linea);
    }


    gettimeofday(&tv_fin, NULL);

    double tiempoOverhead = (tv_inicio.tv_sec - tv_overhead.tv_sec + (tv_inicio.tv_usec - tv_overhead.tv_usec) / 1.e6);
    double tiempoCalculo = (tv_fin.tv_sec - tv_inicio.tv_sec + (tv_fin.tv_usec - tv_inicio.tv_usec) / 1.e6);
    tiempoCalculo -= tiempoOverhead;


    if (cuenta == 2) {
        media /= cuenta;

        printf("Media calculada por el hijo %d: %lf\n", 4, media);
        fprintf(archivo, "Hijo %d: media: %lf\n", 4, media);
        printf("Tiempo utilizado por el hijo %d: %lf\n", 4, tiempoCalculo);
        fprintf(archivo, "Hijo %d: tiempo: %lf\n", 4, tiempoCalculo);
    } else {
        fprintf(stdout, "ERROR: No se han encontrado los datos a procesar.\n");
    }
}


int main() {

//    double y = 0, ymed = 0, conty = 0;//media numeros impares

    //FILE* archivo = fopen("./resultados.log", "a+");
    //FILE* archivo = fopen("./resultados.log", "r+");
    fclose(fopen("./resultados.log", "w"));
    //FILE* archivo = fopen("./resultados.log", "r+");
    FILE *archivo = fopen("./resultados.log", "a+");
    //fseek(archivo, 0, SEEK_SET);

    if (archivo == NULL) {
        perror("No se ha podido acceder correctamente al archivo.");
        exit(-1);
    }

    /* ********** HIJO 1 ********** */

    pid_t pid_hijo1 = fork();

    if (pid_hijo1 == -1) {
        perror("Error: el fork no se ha podido realizar con exito");
        exit(-1);
    }

    if (pid_hijo1 == 0) {
        calculaMediasTangentes(archivo, 1);

        fclose(archivo);

        exit(0);
    }

    /* ********** HIJO 2 ********** */

    //else  if (pid_hijo1 > 0) // En este caso, el proceso que ejecuta el código es el padre, ya que sabe el pid del hijo.

    // no uso un if porque el hijo 1 ya termina con un exit

    pid_t pid_hijo2 = fork();

    if (pid_hijo2 == -1) {
        //todo: ver que hacer con el hijo 1
        perror("Error: el fork no se ha podido realizar con exito");
        exit(-1);
    }

    if (pid_hijo2 == 0) //Esta sección de código es ejecutada por el hijo 2.
    {
        calculaMediasTangentes(archivo, 2);

        fclose(archivo);

        exit(0);
    }

    /* ********** HIJO 3 ********** */

//    if (pid_hijo2 > 0)

    pid_t pid_hijo3 = fork();

    if (pid_hijo3 == -1) {
        //todo: ver que hacer con los hijos 1 y 2
        perror("Error: el fork no se ha podido realizar con exito");
        exit(-1);
    }

    if (pid_hijo3 == 0) {
        calculaMediasTangentes(archivo, 3);

        fclose(archivo);

        exit(0);
    }

    /* ********** PADRE: ESPERANDO A HIJOS 1 Y 2 ********** */

    int status;

    fprintf(stdout, "Esperando hijo 1...\n");
    waitpid(pid_hijo1, &status, 0);
    printf("El hijo 1 ha finalizado con estado: %d\n", WEXITSTATUS(status));

    fprintf(stdout, "Esperando hijo 2...\n");
    waitpid(pid_hijo2, &status, 0);
    printf("El hijo 2 ha finalizado con estado: %d\n", WEXITSTATUS(status));

    /* ********** HIJO 4 ********** */

    //   if (pid_hijo3 > 0)

    pid_t pid_hijo4 = fork();
    if (pid_hijo4 == -1) {
        fprintf(stdout, "\n error: no se pudo hacer el fork");
        exit(-1);
    }
    if (pid_hijo4 == 0) {
        calculaMediaDesdeArchivo(archivo);

        fclose(archivo);

        exit(0);
    }

    /* ********** PADRE: ESPERANDO A HIJOS 3 Y 4 ********** */

    fprintf(stdout, "Esperando hijo 3...\n");
    waitpid(pid_hijo3, &status, 0);
    printf("El hijo 3 ha finalizado con estado: %d\n", WEXITSTATUS(status));

    fprintf(stdout, "Esperando hijo 4...\n");
    waitpid(pid_hijo4, &status, 0);
    printf("El hijo 4 ha finalizado con estado: %d\n", WEXITSTATUS(status));

    /* ********** PADRE ********** */

    fseek(archivo, 0, SEEK_SET);

    char *linea = NULL;
    size_t len = 0;
    ssize_t read;

    double media[] = {0, 0};
    int leidoBien[] = {0, 0};
    int cuenta = 0;
    while ((cuenta < 2) && ((read = getline(&linea, &len, archivo)) != -1)) {
        //printf("Linea recuperada de longitud %zu: %s\n", read, linea);

        if (strncmp(linea, "Hijo 3: media:", 14) == 0) {
            double valor;
            int ok = sscanf(linea + 14, "%lf", &valor);
            if (ok) {
                media[0] = valor;
                cuenta++;
                leidoBien[0] = 1;
            }
        }

        if (strncmp(linea, "Hijo 4: media:", 14) == 0) {
            double valor;
            int ok = sscanf(linea + 14, "%lf", &valor);
            if (ok) {
                media[1] = valor;
                cuenta++;
                leidoBien[1] = 1;
            }
        }
    }
    double diferencia;
    if (leidoBien[0] && leidoBien[1]) {
        diferencia = media[0] - media[1];
        fprintf(stdout, "Diferencia de las dos medias: %lf", diferencia);
    }

    fseek(archivo, 0, SEEK_SET);

    linea = NULL;
    len = 0;
    ssize_t readp;

    double tiempo[] = {0, 0, 0, 0};
    int leidoBienp[] = {0, 0, 0, 0};
    int cuentap = 0;
    while ((cuentap < 4) && ((readp = getline(&linea, &len, archivo)) != -1)) {
        ////printf("Linea recuperada de longitud %zu: %s\n", readp, lineap);

        if (strncmp(linea, "Hijo 1: tiempo:", 15) == 0) {
            double valor;
            int ok = sscanf(linea + 15, "%lf", &valor);
            if (ok) {
                tiempo[0] = valor;
                cuentap++;
                leidoBienp[0] = 1;
            }
        }

        if (strncmp(linea, "Hijo 2: tiempo:", 15) == 0) {
            double valor;
            int ok = sscanf(linea + 15, "%lf", &valor);
            if (ok) {
                tiempo[1] = valor;
                cuentap++;
                leidoBienp[1] = 1;
            }
        }

        if (strncmp(linea, "Hijo 3: tiempo:", 15) == 0) {
            double valor;
            int ok = sscanf(linea + 15, "%lf", &valor);
            if (ok) {
                tiempo[2] = valor;
                cuentap++;
                leidoBienp[2] = 1;
            }
        }

        if (strncmp(linea, "Hijo 4: tiempo:", 15) == 0) {
            double valor;
            int ok = sscanf(linea + 15, "%lf", &valor);
            if (ok) {
                tiempo[3] = valor;
                cuentap++;
                leidoBienp[3] = 1;
            }
        }
    }

    if (leidoBienp[0] && leidoBienp[1] && leidoBienp[2] && leidoBienp[3]) {
        fprintf(stdout, "\nTiempo 1: %lf\n", tiempo[0]);
        fprintf(stdout, "Tiempo 2: %lf\n", tiempo[1]);
        fprintf(stdout, "Tiempo 3: %lf\n", tiempo[2]);
        fprintf(stdout, "Tiempo 4: %lf\n", tiempo[3]);

    }


    if (linea) {
        free(linea);
    }


//    calculaDiferenciaDesdeArchivo(archivo);
    fclose(archivo);
    exit(0);
}
