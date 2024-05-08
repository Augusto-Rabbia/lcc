#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    if (argc != 3) {
        printf("incorrect number of arguments");
        return 1;
    }
    int n = atoi(argv[1]);
    
    for (int i = 0; argv[2][i] != '\0'; i++)
        printf("%c", argv[2][i] ^ n);
    return 0;
}