#include <stdio.h>
#include <stdlib.h>
#include "intarr.h"


intarr* intarr_create(int len) {
    intarr* newarr = malloc(sizeof(intarr));
    newarr->address = malloc(sizeof(int) * len);
    newarr->size = len;
    return newarr;
}

void intarr_destroy(intarr* A) {
    free(A->address);
    free(A);
}

int intarr_read(intarr* arr, int pos) {
    if (pos < arr->size) {
        printf("\n%d", arr->address[pos]);
    }
    else
        printf("\nIndex too high!");
}

void intarr_write(intarr* arr, int pos, int data) {
    if (pos < arr->size) {
        arr->address[pos] = data;
    }
    else
        printf("\nIndex too high!");
}

int intarr_size(intarr* arr) {
    printf("%d", arr->size);
}

void intarr_print(intarr* arr) {
    for (int i = 0; i < arr->size; i++) {
        printf("%d", arr->address[i]);
    }
}

void intarr_resize(intarr* arr, int newsize) {
    arr->address = realloc(arr->address, sizeof(int) * newsize);
    arr->size = newsize;
}

void intarr_insert(intarr* arr, int pos, int data) {
    intarr_resize(arr, (arr->size + 1));
    for(int i = arr->size; i > pos; i--) {
        arr->address[i] = arr->address[i-1];
    }
    arr->address[pos] = data;
}

void intarr_del(intarr* arr, int pos) {
    for(int i = pos; i < arr->size; i++) {
        arr->address[i] = arr->address[i+1];
    }
    intarr_resize(arr, (arr->size -1));
}