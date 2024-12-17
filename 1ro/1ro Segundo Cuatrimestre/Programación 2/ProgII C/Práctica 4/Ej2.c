#include <stdio.h>
#include <stdlib.h>

int strlen2(char *c){
    int len = 0;
    for(int i = 0; c[i] != '\0'; i++){
        len ++;
    }
    printf("%d", len);
    return len;
}

int main(){
    char string[] = "Hello";
    strlen2(string);



    return 0;
}