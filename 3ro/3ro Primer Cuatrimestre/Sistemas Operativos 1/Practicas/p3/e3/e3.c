#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define M 5
#define N 5
#define SZ 8

int *buffer[SZ];

// semaforo para que un producto no se produzca cuando el buffer esta lleno
sem_t existencias;
// semaforo para que no se consuma cuando el buffer esta vacio
sem_t faltantes;

// semaforo para evitar race condition en el buffer
sem_t mercado;


// representa el indice del ultimo producto del buffer
int nProducto = 0;



/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];

void enviar(int *p)
{
    sem_wait(&mercado);
    buffer[nProducto++] = p;
    sem_post(&mercado);
	return;
}

int * recibir()
{
	sem_wait(&mercado);
    int *producto = buffer[(nProducto--)-1];
    sem_post(&mercado);
	return producto;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);
        sem_wait(&faltantes);
		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
        sem_post(&existencias);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);
        sem_wait(&existencias);
		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
        sem_post(&faltantes);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
    sem_init(&faltantes, 0, SZ);
    sem_init(&existencias, 0, 0);
    sem_init(&mercado,0, 1);

	int i;

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}