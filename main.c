#include <stdio.h>
#include <pthread.h>
#define MAX 5                              /* número de ranuras en el bufer */

pthread_mutex_t mi_mutex;                   /* variable mutex para region critica */
pthread_cond_t condc,condp;                 /* variables de condicion para productor y consumidor */
int bufer = 0;                              /* búfer utilizado entre productor y consumidor */

void *productor(void *ptr)                  /* producir datos */
{
    int i;
    for (i=1; i<=MAX; i++) {
        pthread_mutex_lock(&mi_mutex);      /* obtiene acceso exclusivo al búfer */
        printf("Productor %d entrando a region critica\t\tBufer: %d\n", i, bufer);
        while (bufer!=0)
            pthread_cond_wait(&condp, &mi_mutex); /* duerme al productor en espera de una señal */
        bufer = i;                          /* coloca elemento en el búfer */
        printf("Productor %d colocando elemento en el bufer\tBufer: %d\n", i, bufer);
        pthread_cond_signal(&condc);        /* despierta al consumidor */
        printf("Productor %d despertando al consumidor\t\tBufer: %d\n", i, bufer);
        pthread_mutex_unlock(&mi_mutex);    /* libera el acceso al búfer */
        printf("Productor %d liberando acceso al bufer\t\tBufer: %d\n", i, bufer);
    }
    pthread_exit(0);
    return 0;
}

void *consumidor(void *ptr)                 /* consumir datos */
{
    int i;
    for (i=1; i<=MAX; i++) {
        pthread_mutex_lock(&mi_mutex);      /* obtiene acceso exclusivo al búfer */
        printf("Consumidor %d entrando a region critica\t\tBufer: %d\n", i, bufer);
        while (bufer==0)
            pthread_cond_wait(&condc, &mi_mutex); /* duerme al consumidor en espera de una señal */
        bufer = 0;                          /* saca elemento del búfer */
        printf("Consumidor %d sacando elemento del bufer\t\tBufer: %d\n", i, bufer);
        pthread_cond_signal(&condp);        /* despierta al productor */
        printf("Consumidor %d despertando al productor\t\tBufer: %d\n", i, bufer);
        pthread_mutex_unlock(&mi_mutex);    /* libera el acceso al búfer */
        printf("Consumidor %d liberando accceso al bufer\t\tBufer: %d\n", i, bufer);
    }
    pthread_exit(0);
    return 0;
}

int main(int argc, char **argv)
{
    pthread_t pro, con;                         /* hilos productor y consumidor */
    pthread_mutex_init(&mi_mutex, 0);           /* inicia el mutex */
    pthread_cond_init(&condc, 0);               /* inicia mutex para consumidor */
    pthread_cond_init(&condp, 0);               /* inicia mutex para productor */
    pthread_create(&con, 0, consumidor, 0);     /* crea e inicia hilo consumidor con funcion consumidor */
    pthread_create(&pro, 0, productor, 0);      /* crea e inicia hilo productor con funcion productor */
    pthread_join(pro, 0);                       /* espera por el término de un hilo */
    pthread_join(con, 0);                       /* espera por el término de un hilo */
    pthread_cond_destroy(&condc);               /* se destruye variable de condicion para consumidor */
    pthread_cond_destroy(&condp);               /* se destruye variable de condicion para productor */
    pthread_mutex_destroy(&mi_mutex);           /* se destruye el mutex para region critica */
    return 0;
}
