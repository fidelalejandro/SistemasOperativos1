#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

/*
 Haz un programa P que capture las señales SIGUSR1 y SIGUSR2 e ignore la señal SIGINT con
        las siguientes características. ENTREGABLE 2.
• El proceso captura las señales SIGUSR1 y SIGUSR2 e ignora la señal SIGINT (señales que
        posteriormente recibirá de dos procesos hijo) con la llamada al sistema sigaction; de tal
modo que el manejador de las señales SIGUSR1 y SIGUSR2 simplemente debe mostrar en
pantalla un mensaje indicando qué señal se ha recibido. A continuación P crea dos procesos
hijo, H1 y H2, y espera por la terminación del primer hijo H1, mostrando en pantalla el
valor del código enviado a través del exit de ese hijo.
• El primer proceso hijo H1 envía la señal SIGUSR1 al padre, espera 10 segundos, y a continuación envía la señal SIGUSR2 al padre, espera otros 5 segundos y termina.
• Además de lo indicado anteriormente, el proceso padre P debe bloquear la señal SIGUSR1
        durante un tiempo antes de capturarla. Para poder ver el efecto del bloqueo, P debe
bloquear la señal SIGUSR1 antes de que el hijo H1 se la envíe y desbloquearla después de
        que el hijo haya enviado la señal SIGUSR2. Para ello, una vez que se ha bloqueado la señal
        SIGUSR1, se debe dormir el proceso padre durante un tiempo suficiente para que el hijo
        envíe la señal SIGUSR2 antes de que se despierte y desbloquear la señal SIGUSR1 después
de despertarse. Antes de desbloquear la señal, el padre debe comprobar que SIGUSR1
está pendiente (bloqueada). Como resultado, el padre debería procesar la señal SIGUSR2
antes que la SIGUSR1 a pesar de haberla recibido más tarde. Utiliza la llamada al sistema
        sigprocmask para bloquear la señal y las llamadas sigpending y sigismember para comprobar
si están bloqueadas.
• El segundo proceso hijo H2 simplemente envía la señal SIGINT al proceso padre y termina.
Como el padre ignora esta señal, no tendrá como efecto la terminación del proceso padre.
• Todos los procesos involucrados deben imprimir en consola comentarios exhaustivos sobre
        cada una de las acciones que realizan, identificándose y mostrando la hora para comprobar
de una manera clara que la secuencia de acciones es la esperada.
Nota 1: En la estructura tipo sigaction es conveniente activar el flag SA_RESTART, que fuerza
el reiniciado de ciertas llamadas del sistema cuando sean interrumpidas por un gestor de señal;
en caso contrario la llamada al sistema se interrumpiría cuando el proceso recibe la señal.
Nota 2: Se recomienda que primero hagas el programa sin bloquear la señal SIGUSR1, y cuando
        funcione correctamente introduzcas el bloqueo y la comprobación de que está pendiente.*/

void manejador(int sig)
{
    time_t hora;
    time(&hora);
    switch(sig)
    {
        case SIGUSR1:
            printf("Recibida SIGUSR1 a las %s\n",ctime(&hora));
            break;
        case SIGUSR2:
            printf("Recibida SIGUSR2 a las %s\n",ctime(&hora));
            break;
        case SIGINT:
            printf("Ignorando SIGINT a las %s\n",ctime(&hora));
    }
    
}


int main()
{
    struct sigaction siguser;
    siguser.sa_handler = manejador;
    
    struct sigaction sigint;
    sigint.sa_handler = manejador;
    sigint.sa_flags = SA_RESTART;
    
    sigaction(SIGUSR1,&siguser,NULL);
    sigaction(SIGUSR2,&siguser,NULL);
    sigaction(SIGINT,&sigint,NULL);
    
    pid_t P = getpid();
    
    sigset_t sigusr1;
    sigaddset(&sigusr1,SIGUSR1);
    
    sigprocmask(SIG_BLOCK,&sigusr1,NULL);
    if(sigismember(&sigusr1,SIGUSR1))printf("SIGUSR1 está bloqueada\n");
    
    pid_t H1 = fork();
    if (H1 == 0)
    {
        kill(P,SIGUSR1);
        sleep(10);
        kill(P,SIGUSR2);
        sleep(5);
        exit(0);
    }
    
    pid_t H2 = fork();
    if (H2 == 0)
    {
        kill(P,SIGINT);
        exit(0);
    }
    
    
    sleep(20);
    if(sigpending(&sigusr1)==0)printf("Señal esperando\n");
    
    sigprocmask(SIG_UNBLOCK,&sigusr1,NULL);
    
    
    int status;
    waitpid(H1,&status,0);
    printf("Hijo 1 pid %i ha terminado con codigo %i\n",H1, WEXITSTATUS(status));
    
    return 0;
}
