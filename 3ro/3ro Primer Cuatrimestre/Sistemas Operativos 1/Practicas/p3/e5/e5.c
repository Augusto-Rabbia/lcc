#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_SILLAS 5
// cuántos clientes entran por cada tanda (se puede 
// implementar de otras formas)
#define TANDA_DE_CLIENTES 10

// El semáforo representa la cantidad de sillas libres.
sem_t semaphore; 
// Maneja el dormir del barbero, y la espera para asegurar 
// la concurrencia del corte y pago.
pthread_cond_t cv;
// Maneja los turnos.
pthread_cond_t cv_turno[N_SILLAS+1]; 

// Se encarga de sólo un cliente pueda cortarse el pelo a la vez.
pthread_mutex_t mutex_cliente;
// Se asegura que el corte y el pago se realicen 
// paralelamente junto con cv.
pthread_mutex_t esperar_terminar;
// Para evitar race condition en la toma de los turnos de los 
// clientes.
pthread_mutex_t mutex_turno;

int a_cortar = 0, turno = 0;

void me_cortan() {
    printf("Me cortan...\n");
    sleep(1);
}

void pagando() {
    printf("Pagando...\n");
    sleep(1);
}

void * cliente() {
    // Cada cliente intenta tomar una silla, y si no hay ninguna libre, se va.
    if (sem_trywait(&semaphore) == -1) {
        printf("Cliente: \"No hay sillas, chau.\"\n");
        pthread_exit(NULL);
    }
    // Luego de ver si habia sillas, sacamos un turno 
    // y esperamos a que nos deje entrar
    pthread_mutex_lock(&mutex_turno);
    int turnoLocal = turno++;
    printf("Tomo el turno %d!\n", turnoLocal);
    pthread_mutex_unlock(&mutex_turno);
    if (a_cortar != turnoLocal)
        pthread_cond_wait(&cv_turno[turnoLocal % (N_SILLAS+1)], &mutex_cliente);
    
    // Entra un cliente. Despierta al barbero, si este duerme, 
    // y deja su asiento libre.
    pthread_cond_signal(&cv);
    sem_post(&semaphore);

    me_cortan();
    
    // Aseguramos que el corte y pago se hagan juntos para
    // el cliente y barbero.
    pthread_mutex_lock(&esperar_terminar);
    pthread_cond_signal(&cv);
    pthread_cond_wait(&cv, &esperar_terminar);
    pthread_mutex_unlock(&esperar_terminar);
    pthread_cond_signal(&cv);

    pagando();
    //Puede pasar el próximo cliente.
    pthread_mutex_unlock(&mutex_cliente);
    pthread_exit(NULL);
}

void * entran_clientes() {
    pthread_t clientela[TANDA_DE_CLIENTES];
    while(1) {
        // entra cliente
        for(int i = 0; i < TANDA_DE_CLIENTES; i++)
            pthread_create(&clientela[i], NULL, cliente, NULL);
        pthread_join(clientela[0], NULL);
        sleep(10);
    }
}

void cortando() {
    printf("Cortando...\n");
    sleep(1);
}

void me_pagan() {
    printf("Kaching!!\n");
    sleep(1);
}

void * barbero_trabaja(void *arg) {
    int sillas;
    while (1) {
        // Vemos si hay alguien esperando a ser cortado.
        pthread_mutex_lock(&mutex_turno);
        if (turno == a_cortar) {
            printf("No hay clientes, voy a dormir.\n");
            pthread_cond_wait(&cv, &mutex_turno);
        }
        pthread_mutex_unlock(&mutex_turno);
        // Llama a que pase el cliente con el próximo turno. 
        printf("Que pase el %d\n", a_cortar);
        pthread_cond_signal(&cv_turno[a_cortar % (N_SILLAS+1)]);
        
        cortando();
        
        // Aseguramos que el corte y pago se hagan juntos para
        // el cliente y barbero.
        pthread_mutex_lock(&esperar_terminar);
        pthread_cond_signal(&cv);
        pthread_cond_wait(&cv, &esperar_terminar);
        pthread_mutex_unlock(&esperar_terminar);
        pthread_cond_signal(&cv);

        me_pagan();
        printf("Barbero: Próximo cliente!\n");
        a_cortar++;
    }    
}

int main() {
    pthread_t clientes, barbero;
    sem_init(&semaphore, 0, N_SILLAS);

    int i;
    pthread_create(&barbero, NULL, barbero_trabaja, NULL);
    pthread_create(&clientes, NULL, entran_clientes, NULL);
    pthread_join(barbero, NULL);
    
    sem_destroy(&semaphore);
    return 0;
}