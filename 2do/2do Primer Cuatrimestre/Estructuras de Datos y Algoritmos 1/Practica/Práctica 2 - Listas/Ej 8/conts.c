#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "conts.h"

contact contact_create(char* name, char* age) {
    contact new_c;
    new_c.name = malloc(sizeof(char)*20);
    new_c.name = name;
    new_c.age = malloc(sizeof(char)*20);
    new_c.age = age;
    return new_c;
}

void contact_free(contact c) { 
    free(c.name);
    free(c.age);
}