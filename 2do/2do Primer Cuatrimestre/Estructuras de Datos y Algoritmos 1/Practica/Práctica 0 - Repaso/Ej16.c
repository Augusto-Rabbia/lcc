#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* name;
    int phone;
    unsigned int age;
    } Contact;


typedef struct {
    Contact* contacts;
    unsigned int size;
    } ContBook;

Contact create_contact() {
    Contact cont;

    cont.name = malloc(sizeof(char)*20);

    printf("Enter contact name: ");
    int i = 0;
    char c;
    while((c = getchar()) != '\n')
        cont.name[i++] = c;
    cont.name[i] = '\0';

    //printf("%s", cont.name);

    printf("Enter contact phone number: ");
    scanf("%d", &cont.phone);

    printf("Enter contact age: ");
    scanf("%u", &cont.age);

    return cont;
}

void actualizar_edad(Contact* cont) {
    printf("Enter the contact's updated age: ");
    scanf("%u", &(cont->age));
}

void add_contact(ContBook* Book) {
    Contact contToAdd;
    contToAdd = create_contact();

    Book->contacts = realloc(Book->contacts, sizeof(Contact)*(Book->size+1));

    Book->contacts[Book->size] = contToAdd;
    Book->size++;
    //printf("%d", Book->contacts[0].age);
}

int main(void) {
    Contact cont1;
    ContBook Book1;
    ContBook* pBook1 = &Book1;
    pBook1 = malloc(sizeof(ContBook));
    pBook1->size = 0;

    //cont1 = add_contact();
    //actualizar_edad(&cont1);

    //printf("%s, %d, %d\n", cont1.name, cont1.phone, cont1.age);

    add_contact(pBook1);

    printf("%u, %s, %d, %u\n", pBook1->size, pBook1->contacts[0].name, pBook1->contacts[0].phone, pBook1->contacts[0].age);
    system("Pause");
    return 0;
}
