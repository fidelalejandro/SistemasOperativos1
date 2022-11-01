/*
 *  Escribe un programa que cree n procesos hijos, denominados H[1], H[2], ..., H[n]. El valor de n
 *  debe ser pedido al usuario. El proceso padre P simplemente debe esperar a la finalización del
 *  primer hijo H[1] con la llamada waitpid. Cada hijo H[i] debe finalizar al anterior, H[i-1], con kill
 *  cuando el usuario le envíe una señal SIGUSR1 desde la consola. Asegúrate de algún modo que
 *  todos los procesos terminan normalmente.
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>

void manejador(int);

bool matarAnterior = 0;

int main()
{
    
    signal(SIGUSR1, manejador);
//    signal(SIGTERM, manejador);
    int numeroHijos;
    printf("Número de hijos: ");
    scanf("%d", &numeroHijos);
    
    pid_t padre = getpid();
    pid_t H[numeroHijos + 1];
//    pid_t hijoPrevio;
    H[0] = 0; //No se usa.
    int numHijo;
    for (numHijo = 1; numHijo <= numeroHijos; numHijo++)
    {
//        if (numHijo == 1)
//        {
//            hijoPrevio = -1; // Indico al proceso que no mate al previo
//            // printf("En zero\numeroHijos");
//        } else
//        {
//            hijoPrevio = H[numHijo - 1];
//            //printf("Fuera de zero\numeroHijos");
//        }
        H[numHijo] = fork();
        if (H[numHijo] == 0)
        {
            break; //No quiero que los hijos hagan fork().
        }
    }
    
    
    if (getpid() == padre)
    {
        printf("Padre: %i\n", getpid());
        printf("Ultimo hijo: %i\n", H[numeroHijos]);
        
        for (int i = numeroHijos; i > 0; i--)
        {
            int status;
            waitpid(H[i], &status, 0);
            if (WIFEXITED(status))
            {
                printf("Child %d exited with RC=%d\n", H[i], WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status))
            {
                printf("Child %d exited with SIG=%d\n", H[i], WTERMSIG(status));
            }
            
            /*printf("PADRE:: mi pid es: %i y el interruptor esta a: %i\numeroHijos", getpid(), interruptor);
            int status;
            sleep(5);
            kill(H[numeroHijos - 1], SIGUSR1);
            waitpid(H[numeroHijos-1], &status, 0;
            if (WIFEXITED(status))
            {
                printf("\nChild %d exited with RC=%d\numeroHijos",H[numeroHijos-1],WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status))
            {
                printf("\nChild %d exited via signal %d\numeroHijos",H[numeroHijos-1],WTERMSIG(status));
            }
            waitpid(H[0], &status, 0);
            if (WIFEXITED(status))
            {
                printf("Child %d exited with RC=%d\numeroHijos",H[0],WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status))
            {
                printf("Child %d exited via signal %d\numeroHijos",H[0],WTERMSIG(status));
            }
            printf("PADRE:: terminaron mis hijos\numeroHijos");
            exit(0);*/
        }
        printf("Fin\n");
        exit(0);
    } else
    {
        
        while (true)
        {
            printf("[%d] H[%i] Me duermo\n",getpid(),numHijo);
            pause();
            printf("[%d] H[%i] Me despierto\n",getpid(),numHijo);
            if (matarAnterior == true)
            {
                
                if (H[numHijo-1] == 0)
                {
                    printf("[%d] H[%d] Me voy sin hacer nada\n",getpid(),numHijo);
                    exit(0);
                }
                printf("[%d] H[%d] Voy a matar a H[%d]=%d\n",getpid(),numHijo,numHijo-1,H[numHijo-1]);
    
                kill(H[numHijo - 1], SIGUSR1);
                exit(0);
            }
        }
        /*while (1)
        {
            printf("mi pid es: %i y el interruptor esta a: %i\numeroHijos", getpid(), interruptor);
            pause();
            if (interruptor == 1)
            {
                kill(hijoPrevio, SIGUSR1);
                waitpid(hijoPrevio, &status, WEXITED);
                if (WIFEXITED(status))
                {
                    printf("Child %d exited with RC=%d\numeroHijos",hijoPrevio,WEXITSTATUS(status));
                }
                if (WIFSIGNALED(status))
                {
                    printf("Child %d exited via signal %d\numeroHijos",hijoPrevio,WTERMSIG(status));
                }
                int codigo = WTERMSIG(status);
                printf("El proceso %d ha salido con el cod: %i\numeroHijos", hijoPrevio, codigo);
                
                printf("me voy \numeroHijos");
                raise(SIGTERM);
                printf("Sigo aquí\numeroHijos");
            }
        }*/
    }
    
    
}

void manejador(int sn)
{
    switch (sn)
    {
        case SIGUSR1:
             printf("[%d] He recibido SIGUSR1\n",getpid());
            matarAnterior = true;
            break;
    }
    
}
