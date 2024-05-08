#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int* address;
    int size;
    } intArray;

intArray* create_intArray(int size) {
    intArray* arr = malloc(sizeof(intArray));
    arr->address = malloc(sizeof(int)*size);
    arr->size = size;
    return arr;
}

void intArray_destroy(intArray* int_arr) {
    free(int_arr->address);
    free(int_arr);
}

void intArray_write(intArray* int_arr, int pos, int data_input) {
    int_arr->address[pos] = data_input;
}

int intArray_read(intArray* int_arr, int pos) {
    return int_arr->address[pos];
}

int intArray_size(intArray* int_arr) {
    return int_arr->size;
}

void intArray_print(intArray* int_arr) {
    int i = 0;
    printf("Array elements: ");
    while(i < int_arr->size) {
        printf("%d ", int_arr->address[i++]);
    }
    printf("\n");
}

//_______________Ej4

void intArray_adjsut(intArray* int_arr, int new_size) {
    int_arr->address = realloc(int_arr->address, sizeof(int)*new_size);
    int_arr->size = new_size;
}

void intArray_insert(intArray* int_arr, int pos, int data_input) {
    int i = int_arr->size - 1;
    while (i > pos){
        int_arr->address[i] = int_arr->address[i-1];
        i--;
    }
    
    int_arr->address[pos] = data_input;
}

void intArray_del(intArray* int_arr, int pos) {
    int i = int_arr->size - 1;
    while (i > pos) {
        int_arr->address[i-1] = int_arr->address[i];
        int_arr->address[i--] = NULL;
    }
    int_arr->size --;
    int_arr->address = realloc(int_arr->address, sizeof(int)*int_arr->size);
}


int main(void) {   
    int size_arr1 = 1;
    intArray* intArr1 = create_intArray(size_arr1);

    intArray_write(intArr1, 0, 5);

    int elem0 = intArray_read(intArr1, 0);
    
    printf("elem0 = %d\n", elem0);
    printf("Size = %d\n", intArray_size(intArr1));
    intArray_print(intArr1);

    //_______________Ej4
    printf("\nEj4:\n");

    size_arr1 = 2;
    intArray_adjsut(intArr1, size_arr1);

    intArray_insert(intArr1, 0, 4);

    int elem0_new = intArray_read(intArr1, 0);
    
    printf("elem0 = %d\n", elem0_new);
    printf("Size = %d\n", intArray_size(intArr1));
    intArray_print(intArr1);


    intArray_del(intArr1, 0);
    printf("Size = %d\n", intArray_size(intArr1));
    intArray_print(intArr1);

    return 0;
}