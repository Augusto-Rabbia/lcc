#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>


#define M 5
#define N 5
#define SZ 8

int *buffer[SZ];

// representa el indice del ultimo producto del buffer
int nProducto = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cVacio, cLleno;
bool lleno = false, vacio = true;

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];

void enviar(int *p) { buffer[nProducto++] = p; }

int * recibir() {
	int *producto = buffer[(nProducto--)-1];
	return producto;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);
		pthread_mutex_lock(&mutex);
		while(nProducto == SZ - 1) {
			lleno = true;
			pthread_cond_wait(&cLleno, &mutex);
		}
		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
		if (vacio) {
			vacio = false;
			pthread_cond_signal(&cVacio);
		}
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);
		pthread_mutex_lock(&mutex);
		while(nProducto == 0) {
			vacio = true;
			pthread_cond_wait(&cVacio, &mutex);
		}
		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
		if (lleno) {
			lleno = false;
			pthread_cond_signal(&cLleno);
		}
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	pthread_mutex_init(&mutex, NULL);

	int i;

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}