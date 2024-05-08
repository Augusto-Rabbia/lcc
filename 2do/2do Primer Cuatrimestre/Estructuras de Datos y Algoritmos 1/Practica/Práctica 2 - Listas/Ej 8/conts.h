#ifndef __conts_h__
#define __conts_h__

typedef struct _cont {
    char* name;
    char* age;
} contact;

contact contact_create(char* name, int age);

void contact_free(contact c);


#endif