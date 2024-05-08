#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char String[] = "Hello World";
    char strBackwards[strlen(String)];
    for(int i = strlen(String);i >= -1; i--){
        strBackwards[strlen(String)-i-1] = String[i];
    }
    printf("%s backwards is spelled %s", String, strBackwards);
    return 0;
}