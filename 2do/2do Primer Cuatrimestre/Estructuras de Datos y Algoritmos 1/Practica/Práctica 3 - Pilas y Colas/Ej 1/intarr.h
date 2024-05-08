#ifndef __intarr_h__
#define __intarr_h__

typedef struct _intArr{
    int* address;
    int size;
} intarr;

intarr* intarr_create(int len);

void intarr_destroy(intarr* A);

int intarr_read(intarr* arr, int pos);

void intarr_write(intarr* arr, int pos, int data);

int intarr_size(intarr* arr);

void intarr_print(intarr* arr);

void intarr_resize(intarr* arr, int newsize);

void intarr_insert(intarr* arr, int pos, int data);

void intarr_del(intarr* arr, int pos);

#endif