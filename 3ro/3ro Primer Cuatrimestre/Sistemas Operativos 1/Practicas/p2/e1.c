#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool tiene5(int* arr, int len) {
    bool encontrado = false;
    for(int i = 0; i < len; i++) {
        if (arr[i] == 5) {
            encontrado = true;
        }
    }
}

bool tiene5(int* arr, int len) {
    bool encontrado = false;
    for(int i = 0; i < len; i++) {
        encontrado == encontrado || (arr[i] == 5);
    }
}

int main() {
    int len = 5000;
    int arr[len];
    memset(arr, 0, len*sizeof(int));

    arr[4500] = 5;
    tiene5(arr, len);

    return 0;
}

//puede fallar si varios threads ejecutan la funcion tiene5OR a la vez?
// Solo en el caso en que la variable encontrado sea global y sea compartida entre los threads.
// Puede ocurrir que dos threads hagan esa asignacion y determinen que dos arrays distintos tienen 5
// cuando alguno de estos no lo tenga en realidad.
// mentendes