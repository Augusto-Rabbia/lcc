#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define TOTAL 10

int main(){
    char str[TOTAL][20] = {"hola", "chau"};
    for(int i = 0; i!=TOTAL; i++){
        printf("%d\n", str[i]);
    }
    return 0;
}









