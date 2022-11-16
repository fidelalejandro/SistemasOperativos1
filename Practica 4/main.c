#include <stdio.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#define K_NUM_HILOS 3
#define K_TOTAL_ITERACIONES 10000000
#define K_CALCULOS_POR_ITERACION 100
#define DEBUG 0

#define CALCULO_PARCIAL_PI(r) (pow(-1, k) / pow(4.00, k)) * (2.00 / (4 * k + 1) + 2.0 / (4. * k + 2) + 1 / (4. * k + 3))

typedef struct
{
    double sumaEsteHilo;
    int numHilo;
    int totalHilos;
    int numCalculosporIter;
#if(DEBUG == 1)
    int iteracionesRealizadas; //Comprobacion de que hace las its.
#endif
} DatosCalculoEsteHilo;


void *calculoEsteHilo(void *pEntrada)
{
    DatosCalculoEsteHilo *datosCalculoEsteHilo = (DatosCalculoEsteHilo *) pEntrada;

    datosCalculoEsteHilo->sumaEsteHilo = 0.0;
#if(DEBUG == 1)
    datosCalculoEsteHilo->iteracionesRealizadas = 0;
#endif

    for (int valorInicialBucleInterno = datosCalculoEsteHilo->numHilo * datosCalculoEsteHilo->numCalculosporIter;
         valorInicialBucleInterno < K_TOTAL_ITERACIONES; valorInicialBucleInterno += datosCalculoEsteHilo->totalHilos * datosCalculoEsteHilo->numCalculosporIter)
    {
        for (int k = valorInicialBucleInterno; k < (valorInicialBucleInterno + datosCalculoEsteHilo->numCalculosporIter); k++)
        {
            datosCalculoEsteHilo->sumaEsteHilo += CALCULO_PARCIAL_PI(k);

#if(DEBUG == 1)
            datosCalculoEsteHilo->iteracionesRealizadas += 1;//para comprobar las iters.
#endif

        }

//        printf("[%d] %d %.70lf\n", datosCalculoEsteHilo->numHilo, valorInicialBucleInterno, datosCalculoEsteHilo->sumaEsteHilo);
    }

    return NULL;
}

double calcularTiempo(struct timeval *tv_overhead, struct timeval *tv_inicio, struct timeval *tv_fin)
{
    double tiempoOverhead = (tv_inicio->tv_sec - tv_overhead->tv_sec + (tv_inicio->tv_usec - tv_overhead->tv_usec) / 1.e6);
    double tiempoCalculo = (tv_fin->tv_sec - tv_inicio->tv_sec + (tv_fin->tv_usec - tv_inicio->tv_usec) / 1.e6);

    tiempoCalculo -= tiempoOverhead;

    return tiempoCalculo;
}

int main()
{
    pthread_t hilo[K_NUM_HILOS];

    DatosCalculoEsteHilo estructuraArgs[K_NUM_HILOS];

    struct timeval tv_overhead;
    struct timeval tv_inicio;
    struct timeval tv_fin;

    gettimeofday(&tv_overhead, NULL);
    gettimeofday(&tv_inicio, NULL);

    for (int numHilo = 0; numHilo < K_NUM_HILOS; numHilo++)
    {
        estructuraArgs[numHilo].numHilo = numHilo;
        estructuraArgs[numHilo].totalHilos = K_NUM_HILOS;
        estructuraArgs[numHilo].numCalculosporIter = K_CALCULOS_POR_ITERACION;
        estructuraArgs[numHilo].sumaEsteHilo = 0.00;

#if(DEBUG == 1)
        estructuraArgs[numHilo].iteracionesRealizadas = 0; //Comprobacion de que hace las its.
#endif

        pthread_create(&hilo[numHilo], NULL, calculoEsteHilo, (void *) &estructuraArgs[numHilo]);

    }

    for (int numHilo = 0; numHilo < K_NUM_HILOS; numHilo++)
    {
        pthread_join(hilo[numHilo], NULL);
    }

    gettimeofday(&tv_fin, NULL);

    double resultadoMultiHilo = 0;
#if(DEBUG == 1)
    int totItrs=0;
#endif
    for (int numHilo = 0; numHilo < K_NUM_HILOS; numHilo++)
    {
        resultadoMultiHilo += estructuraArgs[numHilo].sumaEsteHilo;
#if(DEBUG == 1)
        totItrs +=estructuraArgs[numHilo].iteracionesRealizadas;
#endif
    }


    double tiempoMultiHilo = calcularTiempo(&tv_overhead, &tv_inicio, &tv_fin);
    printf("Resultado multi-hilo: %.55lf\n", resultadoMultiHilo);
    printf("Tiempo multi-hilo: %lf\n", tiempoMultiHilo);
#if(DEBUG == 1)
    printf("Iteraciones multi-hilo: %i\n", totItrs);  //Comprobacion de que hace las its.
#endif

    gettimeofday(&tv_overhead, NULL);
    gettimeofday(&tv_inicio, NULL);
    double resultadoSecuencial = 0;
    for (int k = 0; k < K_TOTAL_ITERACIONES; ++k)
    {
        resultadoSecuencial += CALCULO_PARCIAL_PI(k);
    }

    gettimeofday(&tv_fin, NULL);

    double tiempoSecuencial = calcularTiempo(&tv_overhead, &tv_inicio, &tv_fin);
    printf("Resultado secuencial: %.55lf\n", resultadoSecuencial);
    printf("Tiempo secuencial: %lf\n", tiempoSecuencial);

    return 0;
}
