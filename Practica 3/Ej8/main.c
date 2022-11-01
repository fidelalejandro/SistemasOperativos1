#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include <limits.h>
#include <stdlib.h>

bool esPrimo(int numero);

void manejador(int senhal);

double raizprimo;

int main()
{
    printf("Mi pid es: %d\n\n",getpid());
    signal(SIGALRM, manejador);
    signal(SIGUSR1, manejador);
    alarm(1);
    for (int i = 0; i < INT_MAX; i++)
    {
        
        if (esPrimo(i) == true)
        {
            raizprimo = sqrt(i);
//            printf("Primo: %i.\n", i);
        }
        //       printf("salgo con %i.\n",i);
        
    }
    return 0;
}

bool esPrimo(int numero)
{
    bool res = false;
    int divs = 3;
    int mitad;
    if (numero >= 5 && ((numero & 0x1) == 1))
    {
        /* Para ahorrar sólo comprobamos si un número es divisible hasta su mitad, ya que 7/2 = 3.xxx, 7/3=2.xxx, 7/4=0.xxx */
        /* cuando empezamos a obtener valores menores que 0, ya no encontraremos más divisores enteros */
        mitad = floor((double) numero / 2);
        do
        {
            res = (numero % divs != 0);  /* ¿ El resto es 0 ? */
//            printf("Divido %d entre %d. Resto %d. Res: %d\n", numero, divs, numero % divs, res);
            divs += 2;
        } while ((res != 0) && (divs <= mitad));
    } else if (numero == 2 || numero == 3)
    {
        return true;
    } else
    {
        res = false;
    }
    
    
    return res;
}

void manejador(int senhal)
{
    switch (senhal)
    {
        case SIGALRM:
            printf("Numero raizprimo = %lf\n", raizprimo);
            alarm(1);
            break;
        case SIGUSR1:
            printf("\n\nAdiós\n");
            exit(0);
    }
}