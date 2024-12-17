#include <stdio.h>
#include <stdlib.h>

#define BookLen 100

struct Contact{
    char name[20];
    int phone;
};

struct Contact createContact(){
    struct Contact contactInput;
    printf("Contact name: ");
    gets(contactInput.name);
    printf("Contact phone number: ");
    scanf("%d", &contactInput.phone);
    return contactInput;
};

struct ContactBook{
    struct Contact AddedContacts[BookLen];
    int Count;
};


void AddToConBook(struct Contact ContactToAdd, struct ContactBook *Book){
    struct ContactBook ConBook = *Book;
    for(int i = 0; i <= BookLen; i++){
        if(ConBook.AddedContacts[i] == NULL){
            ConBook.AddedContacts[i].name = ContactToAdd.name;
            ConBook.AddedContacts[i].phone = ContactToAdd.phone;
        }
    }
};

void ViewBook(struct ContactBook Book){
    printf("\nList of Contacts: (name, phone)\n");
    for(int i = 0; i <= BookLen && Book.AddedContacts[i] != NULL; i++){
        printf("%s, %d\n", Book.AddedContacts[i].name, Book.AddedContacts[i].phone);
    }
};

int main(){
    struct Contact contact1 = createContact();
    printf("%s, %d\n", contact1.name, contact1.phone);

    struct Contact contact2;
    contact2.name = "Jorge", contact2.phone = "2";

    struct ContactBook Book1;
    
    struct Contactbook *pBook1 = &Book1;

    AddToConBook(contact1, pBook1);
    AddToConBook(contact2, pBook1);



    system("Pause");
    return 0;
}

//for(int i = 0; i <= sizeof(AddedContacts)/sizeof(AddedContacts[i]; i++))