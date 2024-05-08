#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * string_copy (char *str, int longitud){
    char *b = malloc(sizeof(char)*longitud);
    b = str;
    return b;
}

int main(){
    char a[12] = "Hello World";
    char *b = string_copy(a, 12);
    printf("%s %s\n", a, b);
    b[0] = 's';
    printf("%s %s\n", a, b);
    return 0;
}
