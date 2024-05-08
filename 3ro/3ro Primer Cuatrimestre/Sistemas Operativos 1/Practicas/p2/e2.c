#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0, y = 0, a = 0, b = 0;

void * foo(void *arg) { x = 1; a = y; return NULL; }

void * bar(void *arg) { y = 1; b = x; return NULL; }

int main() {
    pthread_t t0, t1;
    pthread_create(&t0, NULL, foo, NULL);
    pthread_create(&t1, NULL, bar, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    assert (a || b);
    return 0;
}

// Puede fallar si ocurriera que el compilador hiciera una optimizacion de tipo
// y se reemplazara la variable y por 0 en la funcion foo, y la variable x por 0 en bar.
// Esto se podria arreglar agregandoles el atributo volatile a las variables x e y al momento
// de declararlas, lo cual le da una intruccion al compilador de no hacer este tipo de optimizacioness
// O puede ocurrir que dos procesadores distintos ejecuten los threads al mismo tiempo. Al no
// compartir memoria, esto puede llevar a que la asignación que hizo cada uno no sea visible por
// el otro.