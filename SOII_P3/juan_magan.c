#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define P 3 // Número de productores
#define C 2 // Número de consumidores
#define N 10 // Tamaño del buffer
#define MAX_PROD 20 // Numero maximo de items a producir por cada hilo productor
#define MAX_CONS (P*MAX_PROD)/C // Numero maximo de items a consumir por cada hilo consumidor

pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condc = PTHREAD_COND_INITIALIZER;
pthread_cond_t condp = PTHREAD_COND_INITIALIZER;

int n_A;
int n_B;


//Declaración de los arrays de productores y consumidores
int *arrayA;
int *arrayB;

//Sumas de los arrays de A y B
int sumA = 0;
int sumB = 0;

//contador de elementos de A
int i_A = 0;
//contador de elementos de B
int i_B = 0;

//buffer
int *buffer;
//en caso de que el numero de items a consumir por cada consumidor no sea el mismo, sera el ultimo el que consuma los restantes
int resto = (P * MAX_PROD) % C;

/**
 * Función que produce un elemento aleatorio.
 * @return
 */
int produce_item() {
    srand(time(NULL));
    return rand() % 1000 + 1; // generamos un número aleatorio entre 1 y 1000
}

/**
 * Función que inserta un item en un buffer que se pasa como parámetro.
 * @param buffer
 * @param item
 */
void insert_item(int *buffer, int item) {
    //se aumenta el valor del contador
    buffer[0]++;
    //se inserta el item
    buffer[buffer[0]] = item;
}

/**
 * Función que consume un item de un buffer que se pasa por parámetro.
 * @param buffer
 * @return
 */
int consume_item(int *buffer) {
    //se toma el numero que se encuentra en la posicion del buffer indicada por el puntero
    int num = buffer[buffer[0]];
    //se escribe un -1 en su lugar
    buffer[buffer[0]] = -1;
    //se resta el valor del puntero
    buffer[0]--;
    return num;
}

/**
 * Función que produce e inserta un item en un buffer compartido y, además contribuye al sumatorio de los elementos de un array.
 * @param args
 * @return
 */
void *productor(void *args) {


    //se toma el numero de hilo consumidor
    int *num_hilo;
    num_hilo = args;
    //contador de numero de items producidos
    int producidos = 0;
    //tan solo se producen items mientras no se supere el numero maximo (20)
    //while (producidos < MAX_PROD && suma no acabe) {
    while (producidos < MAX_PROD) {

        srand(time(NULL));

        //Se comprueba si el mutex está bloqueado
        if (pthread_mutex_trylock(&the_mutex) == 0) {
            /********************************_____REGIÓN CRÍTICA_____********************************/
            //se solicita el bloqueo del mutex // en el trylock
            //pthread_mutex_lock(&the_mutex);
            //mientras el buffer este lleno (buffer[0]=N), el hilo se bloquea esperando que se produzca un cambio por parte de un hilo consumidor
            while (buffer[0] == N) pthread_cond_wait(&condp, &the_mutex);
            //se produce el item
            int item = produce_item();
            //se inserta el item en el buffer
            insert_item(buffer, item);
            //se ha producido un item más
            producidos++;
            printf("Productor %d produjo %d en la iteracion %d\n", (*num_hilo) + 1, item, producidos);
            //si se produjo un item, se despierta a los posibles hilos consumidores bloqueados porque no hubiese items a consumir en el buffer
            pthread_cond_signal(&condc);
            //se libera el acceso al mutex
            pthread_mutex_unlock(&the_mutex);
            //sleep(1);
            //Comprobamos si el numero de consumidores termina
            if (producidos == MAX_PROD - 1) {
                printf("Acaban de producir los productores\n");
            }
        } else { //FALLA EL TRYLOCK, mutex bloqueado y hay q hacer el sumatorio
            /********************************_____FUERA DE REGIÓN CRÍTICA_____********************************/

            while (i_A < 100 * P) {
                srand(time(NULL));
                n_A = rand() %5 +1;
                for (int i = 0; i < n_A; ++i) {
                    sumA += arrayA[i_A];
                    if(i_A == (100*P)-1) printf("\n*****ACABA LA SUMA DE LOS PRODUCTORES*****\n\n");
                    ++i_A;
                }

            }

        }
    }

    //ACABAN LOS PRODUCTORES
    while (i_A < 100 * P) {
        srand(time(NULL));
         n_A = rand() %5 +1;
        for (int i = 0; i < n_A; ++i) {
            sumA += arrayA[i_A];
            if(i_A == (100*P)-1) printf("\n*****ACABA LA SUMA DE LOS PRODUCTORES*****\n\n");
            ++i_A;
        }
    }

    pthread_exit(NULL);
}

/**
 * Función que consume elementos de un buffer compartido y, además contribuye al sumatorio de los elementos de un array.
 * @param args
 * @return
 */
void *consumidor(void *args) {

    //se toma el numero de hilo consumidor
    int *num_hilo;
    num_hilo = args;
    //contador de items consumidos por el hilo
    int consumidos = 0;
    //el hilo consume mientras no alcanza el maximo
    //while (producidos < MAX_PROD && suma no acabe) {
    while (consumidos < MAX_CONS) { //En teoria esto no se puede hacer
        //Se comprueba si el mutex está bloqueado
        srand(time(NULL));
        if (pthread_mutex_trylock(&the_mutex) == 0) {

            /********************************_____REGIÓN CRÍTICA_____********************************/
            //se solicita el bloqueo del mutex en el trylock
            //pthread_mutex_lock(&the_mutex);
            //mientras el buffer este vacio(buffer[0]=0), el hilo consumidor se bloquea hasta que un hilo productor lo despierte
            while (buffer[0] == 0) pthread_cond_wait(&condc, &the_mutex);
            //se consume el item
            int item = consume_item(buffer);
            //el hilo ha consumido un item mas
            consumidos++;
            printf("Consumidor %d consumio %d en la iteracion %d\n", (*num_hilo) + 1, item, consumidos);
            //si se consume un item, se despierta a los posibles hilos productores bloqueados porque el buffer estuviese lleno
            pthread_cond_signal(&condp);
            //se libera el acceso al mutex
            pthread_mutex_unlock(&the_mutex);
        } else { //FALLA EL TRYLOCK, mutex bloqueado y hay q hacer el sumatorio
            /********************************_____FUERA DE REGIÓN CRÍTICA_____********************************/
            while (i_B < 100 * C) {
                srand(time(NULL));
                n_B = rand() % 5 + 1;
                for (int i = 0; i < n_B; ++i) {
                    sumB += arrayB[i_B];
                    if (i_B == (100 * C) - 1) printf("\n*****ACABA LA SUMA DE LOS CONSUMIDORES*****\n\n");
                    ++i_B;
                }
            }
        }

    }
    //en caso de que el resto de los items a consumir por cada consumidor no sea 0, el último consumidor consumirá los items restantes
    if ((*num_hilo == C - 1) && (resto != 0)) {
        //se pone la variable consumidos a 0
        consumidos = 0;
        //se consumen los items restantes de la division
        while (consumidos < resto) {
            //se solicita el bloqueo del mutex
            pthread_mutex_lock(&the_mutex);
            //mientras el buffer este vacio(buffer[0]=0), el hilo consumidor se bloquea hasta que un hilo productor lo despierte
            while (buffer[0] == 0) pthread_cond_wait(&condc, &the_mutex);
            //se consume el item
            int item = consume_item(buffer);
            //el hilo ha consumido un item mas
            consumidos++;
            printf("Consumidor %d consumio %d en la iteracion %d\n", (*num_hilo) + 1, item, consumidos);
            //si se consume un item, se despierta a los posibles hilos productores bloqueados porque el buffer estuviese lleno
            pthread_cond_signal(&condp);
            //se libera el acceso al mutex
            pthread_mutex_unlock(&the_mutex);
        }
    }

    //ACABAN LOS CONSUMIDORES
    while (i_B < 100 * C) {
        srand(time(NULL));
        n_B = rand() % 5 + 1;
        for (int i = 0; i < n_B; ++i) {
            sumB += arrayB[i_B];
            if (i_B == (100 * C) - 1) printf("\n*****ACABA LA SUMA DE LOS CONSUMIDORES*****\n\n");
            ++i_B;
        }
    }
    pthread_exit(NULL);
}

int main() {

    //Eliminamos las variables de condición antes de crearlas
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);

    //Eliminamos los mutex
    pthread_mutex_destroy(&the_mutex);
    pthread_t productores[P], consumidores[C];

    //Creamos dos vectores para poder pasar a la funcion productor y consumidor. AsÍ evitamos que los hilos modifiquen el valor de una misma variable
    int vector_productores[P];
    int vector_consumidores[C];

    //Inicializamos el mutex
    pthread_mutex_init(&the_mutex, NULL);
    //Inicializamos las variables de condicion de consumo y producción
    pthread_cond_init(&condc, NULL);
    pthread_cond_init(&condp, NULL);

    //Asignamos la memoria a los arrays cuyos elementos se van a sumar
    arrayA = (int *) malloc(100 * P * sizeof(int));
    arrayB = (int *) malloc(100 * C * sizeof(int));

    //Asignamos memoria al puntero
    buffer = (int *) malloc((N + 1) * sizeof(int));
    //el puntero se encuentra en primera posicion
    buffer[0] = 0;
    //Se inicializa el buffer a -1
    for (int i = 1; i < N + 1; i++) {
        buffer[i] = -1;
    }

    //Asignamos valores para arrayA
    for (int i = 0; i < 100 * P; ++i) {
        arrayA[i] = i;
    }
    //Asignamos valores para arrayB
    for (int i = 0; i < 100 * C; ++i) {
        arrayB[i] = i;
    }

    //Creamos los productores
    for (int i = 0; i < P; ++i) {
        vector_productores[i] = i;
        pthread_create(&productores[i], NULL, (void *) productor, (void *) &vector_productores[i]);
    }

    //Creamos los consumidores
    for (int i = 0; i < C; ++i) {
        vector_consumidores[i] = i;
        pthread_create(&consumidores[i], NULL, (void *) consumidor, (void *) &vector_consumidores[i]);
    }

    //Esperamos a que terminen todos los hilos
    for (int i = 0; i < P; ++i) pthread_join(productores[i], NULL);
    for (int i = 0; i < C; ++i) pthread_join(consumidores[i], NULL);

    printf("\n___________FINAL DEL PROGRAMA, IMPRESIÓN DE SUMATORIOS___________\n\n");
    //Imprimimos el valor del sumatorio del arrayA
    printf("La suma de A[] es %d\n", sumA);
    //Imprimimos el valor del sumatorio del arrayB
    printf("La suma de B[] es %d\n", sumB);


    //Eliminamos las variables de condicion
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);

    //Eliminamos el mutex
    pthread_mutex_destroy(&the_mutex);
}