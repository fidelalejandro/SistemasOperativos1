/*
 *  Escribe un programa que cree n procesos hijos, denominados H[1], H[2], ..., H[n]. El valor de n
 *  debe ser pedido al usuario. El proceso padre P simplemente debe esperar a la finalización del
 *  primer hijo H[1] con la llamada waitpid. Cada hijo H[i] debe finalizar al anterior, H[i-1], con kill
 *  cuando el usuario le envíe una señal SIGUSR1 desde la consola. Asegúrate de algún modo que
 *  todos los procesos terminan normalmente.
 */


/*
 * El programa está pensado para que siempre sea el último hijo el que reciba la señal SIGUSR1 y, a partir de este,
 * se vayan finalizando todos los hijos de la forma en la que cada hijo finalice al anterior.
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

#define true 1

void manejador(int);

int matarAnterior =0;

int main()
{
    
    signal(SIGUSR1, manejador);
//    signal(SIGTERM, manejador);
    int n;
    printf("N: ");
    scanf("%d", &n);
    
    pid_t padre = getpid();
    pid_t H[n];
    pid_t hijoPrevio;
    
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            hijoPrevio = -1;
           // printf("En zero\n");
        } else
        {
            hijoPrevio = H[i - 1];
            //printf("Fuera de zero\n");
        }
        H[i] = fork();
        if (H[i] == 0)
        {
            break;
        }
    }
    
    
    if (getpid() == padre)
    {
        printf("Padre: %i\n",getpid());
        printf("Ultimo hijo: %i\n",H[n-1]);
        
        for (int i = n - 1; i > -1; i--)
        {
            int status;
            waitpid(H[i], &status, 0);
            if (WIFEXITED(status))
            {
                printf("Child %d exited with RC=%d\n", H[i],WEXITSTATUS(status));
            }if (WIFSIGNALED(status))
            {
                printf("Child %d exited with SIG=%d\n",H[i], WSTOPSIG(status));
            }
            
            /*printf("PADRE:: mi pid es: %i y el interruptor esta a: %i\n", getpid(), interruptor);
            int status;
            sleep(5);
            kill(H[n - 1], SIGUSR1);
            waitpid(H[n-1], &status, 0;
            if (WIFEXITED(status))
            {
                printf("\nChild %d exited with RC=%d\n",H[n-1],WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status))
            {
                printf("\nChild %d exited via signal %d\n",H[n-1],WTERMSIG(status));
            }
            waitpid(H[0], &status, 0);
            if (WIFEXITED(status))
            {
                printf("Child %d exited with RC=%d\n",H[0],WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status))
            {
                printf("Child %d exited via signal %d\n",H[0],WTERMSIG(status));
            }
            printf("PADRE:: terminaron mis hijos\n");
            exit(0);*/
        }
        printf("Fin\n");
        exit(0);
    } else
    {
        pause();
        
        while (true)
        {
            if(matarAnterior==1)
            {
                
                if(hijoPrevio==-1)
                {
                    exit(0);
                }
                kill(H[n-1],SIGUSR1);
                exit(0);
            }
        }
        /*while (1)
        {
            printf("mi pid es: %i y el interruptor esta a: %i\n", getpid(), interruptor);
            pause();
            if (interruptor == 1)
            {
                kill(hijoPrevio, SIGUSR1);
                waitpid(hijoPrevio, &status, WEXITED);
                if (WIFEXITED(status))
                {
                    printf("Child %d exited with RC=%d\n",hijoPrevio,WEXITSTATUS(status));
                }
                if (WIFSIGNALED(status))
                {
                    printf("Child %d exited via signal %d\n",hijoPrevio,WTERMSIG(status));
                }
                int codigo = WTERMSIG(status);
                printf("El proceso %d ha salido con el cod: %i\n", hijoPrevio, codigo);
                
                printf("me voy \n");
                raise(SIGTERM);
                printf("Sigo aquí\n");
            }
        }*/
    }
    
    
}

void manejador(int sn)
{
    switch (sn)
    {
        case SIGUSR1:
           // printf("He recibido SIGUSR1\n");
            matarAnterior = 1;
            break;
    }
    
}

