//
// Created by alejandro on 12/04/23.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 20 // Numero maximo de numeros a producir
#define P 3 // Número de productores
#define C 2 // Número de consumidores
#define N 20 // Tamaño del buffer
//#define resto 0 //En caso de que el número de items a consumir sea impar es necesaria esta variable para que uno de los consumidores consuma los items restantes
int resto = (P * MAX) % C;

pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condc = PTHREAD_COND_INITIALIZER;
pthread_cond_t condp = PTHREAD_COND_INITIALIZER;
//int buffer = 0; // contador buffer entre el productor y el consumidor
//int consumidos = 0; // contador de numeros producidos
int bufferx[N];

int produce_item() {
    return rand() % 1000 + 1; // generamos un número aleatorio entre 1 y 1000 //todo cambaiar en el 1 entregar
}

void insert_item(int *buffer, int item) {
    int indice = buffer[0];
    buffer[indice + 1] = item;
}

int consume_item(int *buffer) {
    int aux;
    int indice = buffer[0];
    aux = buffer[indice];
    buffer[indice] = 0;
    /*
    if(indice > 1){
        int suma = 0;
        suma += buffer[indice-1]+aux;
        printf("Suma = %d\n", suma);
    }
     */
    return aux;
}

void *productor(void *arg) {
    int producidos = 0; // contador de numeros producidos
    int id = *(int *) arg;
    int producidos_id[P];
    while (producidos_id[id] < MAX) {
        pthread_mutex_lock(&the_mutex);
        while (bufferx[bufferx[0]] != 0) pthread_cond_wait(&condp, &the_mutex);
        if (producidos_id[id] < MAX) {
            int item = produce_item();
            insert_item(bufferx, item);
            //bufferx[bufferx[0]]= ++producidos;
            bufferx[0]++;
            //producidos++;
            producidos_id[id]++;
            printf("Productor %d produjo %d es su %d elemento\n", id, item, producidos_id[id]);
            printf("\n---Productor %d produjo %d\n", bufferx[0], item);

            if (bufferx[0] == 10) {
                pthread_cond_signal(&condc); // despierta al consumidor
                pthread_mutex_unlock(&the_mutex);

            }
        }
        pthread_mutex_unlock(&the_mutex); // libera el acceso al buffer
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) {
    int id = *(int *) arg;
    //bucle con la formula de numeros producidos

    if ((id == C - 1) && (resto != 0)) {
        do {
            pthread_mutex_lock(&the_mutex);
            while (bufferx[0] == 0) pthread_cond_wait(&condc, &the_mutex);
            bufferx[bufferx[0]] = 0;//quitamos elemento todo si tal cambiar mas adelante con las funciones anteriores
            bufferx[0]--;
            if (bufferx[0] == 0) {
                pthread_cond_signal(&condp); // despierta al consumidor
                pthread_mutex_unlock(&the_mutex);

            }
        } while (bufferx[0] != 0);
    } else {
        for (int i = 0; i < id * MAX; ++i) {


            pthread_mutex_lock(&the_mutex);
            while (bufferx[0] == 0) pthread_cond_wait(&condc, &the_mutex);
            bufferx[bufferx[0]] = 0;//quitamos elemento todo si tal cambiar mas adelante con las funciones anteriores
            bufferx[0]--;
            if (bufferx[0] == 0) {
                pthread_cond_signal(&condp); // despierta al consumidor
                pthread_mutex_unlock(&the_mutex);

            }
        }
    }

    pthread_exit(NULL);
}


int main() {
    pthread_t productores[P], consumidores[C], impar;
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condc, NULL);
    pthread_cond_init(&condp, NULL);

    int *arrayA = (int *) malloc(100 * P * sizeof(int));
    int *arrayB = (int *) malloc(100 * C * sizeof(int));

    //int resto=(P*MAX)%C;

    //inicializamos el puntero
    bufferx[0] = 0;
    for (int i = 0; i < N; i++) {
        bufferx[i] = -1;
    }

    //Asignamos valores para arrayA
    for (int i = 0; i < 100 * P; ++i) {
        arrayA[i] = i;
    }
    //Asignamos valores para arrayB
    for (int i = 0; i < 100 * C; ++i) {
        arrayB[i] = i;
    }

    // Creamos los productores
    for (int i = 0; i < P; ++i) {
        pthread_create(&productores[i], NULL, productor, NULL);
    }

    // Creamos los consumidores
    for (int i = 0; i < C; ++i) {
        pthread_create(&consumidores[i], NULL, consumidor, &i);
    }

    // Esperamos a que terminen todos los hilos
    for (int i = 0; i < P; ++i) pthread_join(productores[i], NULL);
    for (int i = 0; i < C; ++i) pthread_join(consumidores[i], NULL);

    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}
