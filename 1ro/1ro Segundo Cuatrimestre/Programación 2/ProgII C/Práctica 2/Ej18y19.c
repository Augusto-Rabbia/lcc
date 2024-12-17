#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int charinStr(char C, char Str[]){
    for(int i = 0; i < strlen(Str); i++){
        if(Str[i] == C){
            return 1;
        }
    }
    return 0;
}

int countCharinStr(char C, char Str[]){
    int count = 0;
    for(int i = 0; i < strlen(Str); i++){
        if(Str[i] == C){
            count++;
        }
    }
    return count;
}

int main(){
    char letter = 'o', String[] = "Hello World";

    if(charinStr(letter, String)){
        printf("The letter is in the string.\n\n");
    }
    else{
        printf("The letter is not in the string.\n\n");
    }

    printf("The letter \"%c\" appears %d times in \"%s\"\n\n", letter, countCharinStr(letter, String), String);
    return 0;
}