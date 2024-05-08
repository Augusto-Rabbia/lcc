#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez;
pthread_cond_t f1, f2, f3, c1, c2, c3, contMain;
pthread_mutex_t mutexCoord, fumadoresListos;

int coord = 0;
int fumadoresDentro = 0;

/* No modificar */
void agente() {
    while (1) {
        sem_wait(&otra_vez);
        int caso = random() % 3;
        printf("Caso: %d\n", caso);
        if (caso != 0) sem_post(&fosforos);
        if (caso != 1) sem_post(&papel);
        if (caso != 2) sem_post(&tabaco);
    }
}
void fumar(int fumador) {
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
    sleep(1);
}

// A esta función se entrará sólo una vez se hayan liberado dos recursos.
void despertar() {
    // Para que no pueda ejecutarse sin que algún 
    // fumador esté esperando a ser despertado
    pthread_mutex_lock(&fumadoresListos); 
    int hayFosforos;
    int hayPapel;
    int hayTabaco;
    // Verificamos cuáles de los recursos están disponibles
    sem_getvalue(&fosforos, &hayFosforos);
    sem_getvalue(&papel, &hayPapel);
    sem_getvalue(&tabaco, &hayTabaco);

    // Despertamos al fumador que necesitaba exactamente los dos recursos
    // que están disponibles.
    if (hayFosforos) {
        if (hayPapel) {
            pthread_cond_signal(&f3);
        }
        else {
            pthread_cond_signal(&f2);
        }
    }
    else pthread_cond_signal(&f1);

    pthread_mutex_unlock(&fumadoresListos);
}

void entraFumador() {
    pthread_mutex_lock(&fumadoresListos);
    fumadoresDentro++;
    if (fumadoresDentro == 3)
        pthread_cond_signal(&contMain);
}

void * fumador1(void *arg) {
    entraFumador();
    while (1) {
        // Espera a que un coordinador le indique que están sus recursos
        // disponibles
        pthread_cond_wait(&f1, &fumadoresListos);
        sem_wait(&tabaco);
        sem_wait(&papel);
        fumar(1);
        sem_post(&otra_vez);
        // Despierta a los coordinadores de papel y tabaco
        pthread_cond_signal(&c2);
        pthread_cond_signal(&c3);
    }
}

void * fumador2(void *arg) {
    entraFumador();
    while (1) {
        // Espera a que un coordinador le indique que están sus recursos
        // disponibles
        pthread_cond_wait(&f2, &fumadoresListos);
        sem_wait(&fosforos);
        sem_wait(&tabaco);
        fumar(2);
        sem_post(&otra_vez);
        // Despierta a los coordinadores de fósforos y tabaco
        pthread_cond_signal(&c1);
        pthread_cond_signal(&c3);
    }
}

void * fumador3(void *arg) {
    entraFumador();
    while (1) {
        // Espera a que un coordinador le indique que están sus recursos
        // disponibles.
        pthread_cond_wait(&f3, &fumadoresListos); // Espera a que
        sem_wait(&papel);
        sem_wait(&fosforos);
        fumar(3);
        sem_post(&otra_vez);
        // Despierta a los coordinadores de fósforos y papel 
        pthread_cond_signal(&c1);
        pthread_cond_signal(&c2);
    }
}

void *coordFosforos(void * arg) {
    while (1) {
        // Esperamos a que esté libre el recurso. Pero cuando esté,
        // lo soltamos para que lo pueda tomar el fumador.
        sem_wait(&fosforos);
        sem_post(&fosforos);
        pthread_mutex_lock(&mutexCoord);
        coord++;
        if (coord == 2) {
            despertar();
            coord = 0;
            pthread_cond_wait(&c1, &mutexCoord);
            pthread_mutex_unlock(&mutexCoord);
        }
        else  {
            pthread_cond_wait(&c1, &mutexCoord);
            pthread_mutex_unlock(&mutexCoord);
        }
    }
}

void *coordPapel(void * arg) {
    while (1) {
        // Esperamos a que esté libre el recurso. Pero si vemos que sí está,
        // lo soltamos para que lo pueda tomar el fumador.
        sem_wait(&papel);
        sem_post(&papel);
        pthread_mutex_lock(&mutexCoord);
        coord++;
        if (coord == 2) {
            despertar();
            coord = 0;
            pthread_cond_wait(&c2, &mutexCoord);
            pthread_mutex_unlock(&mutexCoord);
        }
        else  {
            pthread_cond_wait(&c2, &mutexCoord);
            pthread_mutex_unlock(&mutexCoord);
        }
        
    }
}

void *coordTabaco(void * arg) {
    while (1) {
        // Esperamos a que esté libre el recurso. Pero si vemos que sí está,
        // lo soltamos para que lo pueda tomar el fumador.
        sem_wait(&tabaco);
        sem_post(&tabaco);
        // Este lock protege la variable coord.
        pthread_mutex_lock(&mutexCoord);
        coord++;
        if (coord == 2) { // Significa que ya se liberaron 2 recursos
            despertar();
            coord = 0;
            // Duerme hasta que el recurso correspondiente se vuelva a liberar
            pthread_cond_wait(&c3, &mutexCoord); 
            pthread_mutex_unlock(&mutexCoord);
        }
        else  {
            // Si sólo se liberó un recurso, duerme y libera el
            // lock de coord, hasta que sean utilizados los dos recursos
            pthread_cond_wait(&c3, &mutexCoord);
            pthread_mutex_unlock(&mutexCoord);
        }
        
    }
}

int main() {
    pthread_t s1, s2, s3, sc1, sc2, sc3;
    sem_init(&tabaco, 0, 0);
    sem_init(&papel, 0, 0);
    sem_init(&fosforos, 0, 0);
    sem_init(&otra_vez, 0, 1);
    // Queremos que los fumadores entren ANTES que los coordinadores.
    // de otra manera, puede que los coordinadores envíen la señal
    // antes de que los fumadores la esperen.
    pthread_mutex_lock(&fumadoresListos);
    pthread_create(&s1, NULL, fumador1, NULL);
    pthread_create(&s2, NULL, fumador2, NULL);
    pthread_create(&s3, NULL, fumador3, NULL);
    pthread_cond_wait(&contMain, &fumadoresListos);
    pthread_mutex_unlock(&fumadoresListos);
 
    pthread_create(&sc1, NULL, coordFosforos, NULL);
    pthread_create(&sc2, NULL, coordPapel, NULL);
    pthread_create(&sc3, NULL, coordTabaco, NULL);

    agente();
    return 0;
}
