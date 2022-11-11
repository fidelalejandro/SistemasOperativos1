#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
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

/**
 *
 * @return
 */
char *generarInfoTraza()
{
    pid_t miPid = getpid();

    struct timeval tv_horaActual;
    gettimeofday(&tv_horaActual, NULL);

    struct tm *tm_horaActual = localtime(&tv_horaActual.tv_sec);

    static char bufferTraza[100];
    strftime(bufferTraza, 100, "%F %T.", tm_horaActual);

    sprintf(bufferTraza + 20, "%06ld - pid:% 5d", tv_horaActual.tv_usec, miPid);

    return bufferTraza;
}

void manejador(int sig)
{
    char *infoTraza = generarInfoTraza();

    switch (sig)
    {
        case SIGUSR1:
            printf("[%s] Recibida SIGUSR1\n", infoTraza);
            break;
        case SIGUSR2:
            printf("[%s] Recibida SIGUSR2\n", infoTraza);
            break;
        case SIGINT:
            printf("[%s] Ignorando SIGINT\n", infoTraza);
            break;
        default:
            printf("[%s] recibida la señal %d no esperada.\n", infoTraza, sig);
    }
}


int main()
{
    struct sigaction sa_signal;
    sa_signal.sa_handler = manejador;
    sa_signal.sa_flags = SA_RESTART;

    int signal_success;
    signal_success = sigaction(SIGUSR1, &sa_signal, NULL);
    if (signal_success == -1)
    {
        perror("La asignacion de la señal no ha tenido exito.");
    }
    signal_success = sigaction(SIGUSR2, &sa_signal, NULL);
    if (signal_success == -1)
    {
        perror("La asignacion de la señal no ha tenido exito.");
    }

    signal_success = sigaction(SIGINT, &sa_signal, NULL);
    if (signal_success == -1)
    {
        perror("La asignacion de la señal no ha tenido exito.");
    }

    pid_t pid_Padre = getpid();

    sigset_t ss_sigusr1;
    sigset_t ss_oldSet;
    if (sigaddset(&ss_sigusr1, SIGUSR1) == -1)
    {
        perror("Error en sigaddset()");
    }

    printf("[%s] Padre: Procedo a bloquear SIGUSR1.\n", generarInfoTraza());
    if (sigprocmask(SIG_BLOCK, &ss_sigusr1, NULL) == -1)
    {
        perror("Error en sigprocmask()");
    }
    if (sigprocmask(SIG_SETMASK, NULL, &ss_oldSet) == -1)
    {
        perror("Error en sigprocmask()");
    }
    if (sigismember(&ss_oldSet, SIGUSR1))
    {
        printf("[%s] Padre: SIGUSR1 está bloqueada.\n", generarInfoTraza());
    } else
    {
        printf("[%s] Padre: SIGUSR1 no está bloqueada.\n", generarInfoTraza());
    }

    printf("[%s] Padre: Procedo a hacer el fork del hijo 1.\n", generarInfoTraza());
    pid_t pid_hijo1 = fork();
    if (pid_hijo1 == -1)
    {
        perror("El fork no ha tenido exito.");
        exit(-1);
    }

    if (pid_hijo1 == 0)
    {
        printf("[%s] Hijo1: Enviando señal(SIGUSR1) a padre.\n", generarInfoTraza());
        kill(pid_Padre, SIGUSR1);

        printf("[%s] Hijo1: Voy a dormir durante 10s.\n", generarInfoTraza());
        sleep(10);

        printf("[%s] Hijo1: Enviando señal(SIGUSR2) a padre.\n", generarInfoTraza());
        kill(pid_Padre, SIGUSR2);

        printf("[%s] Hijo1: Voy a dormir durante 5s.\n", generarInfoTraza());
        sleep(5);

        printf("[%s] Hijo1: Hago exit(123).\n", generarInfoTraza());
        exit(123);
    }


    printf("[%s] Padre: Procedo a hacer el fork del hijo 2.\n", generarInfoTraza());
    pid_t pid_hijo2 = fork();
    if (pid_hijo2 == -1)
    {
        perror("El fork no ha tenido exito.");
        exit(-1);
    }
    if (pid_hijo2 == 0)
    {
        printf("[%s] Hijo2: Voy a dormir durante 20s.\n", generarInfoTraza());
        sleep(20);

        printf("[%s] Hijo2: Enviando señal(SIGINT) a padre.\n", generarInfoTraza());
        kill(pid_Padre, SIGINT);

        printf("[%s] Hijo2: Hago exit(0).\n", generarInfoTraza());
        exit(0);
    }

    printf("[%s] Padre: Voy a dormir durante 100s.\n", generarInfoTraza());
    sleep(100);

    sigset_t ss_pending;
    if (sigpending(&ss_pending) == -1)
    {
        perror("Error en sigpending()");
    }

    int ret = sigismember(&ss_pending, SIGUSR1);
    switch (ret)
    {
        case 1:
            printf("[%s] PADRE: Señal SIGUSR1 pendiente de procesarse\n", generarInfoTraza());
            break;
        case 0:
            printf("[%s] PADRE: Señal SIGUSR1 no está pendiente de procesarse\n", generarInfoTraza());
            break;
        default:
            perror("Error en sigismember().");
    }

    printf("[%s] PADRE: Procedo a desbloquear SIGUSR1\n", generarInfoTraza());
    if (sigprocmask(SIG_UNBLOCK, &ss_sigusr1, NULL) == -1)
    {
        perror("Error en sigprocmask()");
    }

    printf("[%s] PADRE: Espero al hijo 1.\n", generarInfoTraza());
    int status;
    waitpid(pid_hijo1, &status, 0);

    if (WIFEXITED(status))
    {
        printf("[%s] Padre: Hijo 1 pid %d ha terminado con RC=%d\n", generarInfoTraza(), pid_hijo1,
               WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status))
    {
        printf("[%s] Padre: Hijo 1 pid %d ha salido con SIG=%d\n", generarInfoTraza(), pid_hijo1, WTERMSIG(status));
    }

    printf("[%s] PADRE: Espero al hijo 2 para salir del programa.\n", generarInfoTraza());
    waitpid(pid_hijo2, NULL, 0);

    printf("[%s] PADRE: Finalizo el programa.\n", generarInfoTraza());
    return 0;
}