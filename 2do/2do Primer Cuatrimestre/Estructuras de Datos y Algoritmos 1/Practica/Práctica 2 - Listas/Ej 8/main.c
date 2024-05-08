#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SGList8.h"
#include "conts.h"



int main(void) {
    SGList l1 = sglist_create();
    contact contacts[5];
    contacts[0] = contact_create("Juan", 15);
    contacts[1] = contact_create("Alan", 32);
    contacts[2] = contact_create("Jose", 24);
    contacts[3] = contact_create("Pablo", 55);
    contacts[4] = contact_create("Sara", 14);
    printf("%s, %s", contacts[0].name, contacts[0].age);

    

    return 0;
}