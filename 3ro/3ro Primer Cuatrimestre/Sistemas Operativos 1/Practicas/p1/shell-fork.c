#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    char input[256];
    char c;
    int k;
    int i;
    while(1) {
        printf("> ");
        fgets(input, 255, stdin);
        input[strlen(input)-1] = '\0';
        char **args = malloc(sizeof(char*)*10);
        for(int j = 0; j < 10; j++) {
            args[j] = calloc(15, sizeof(char));
        }
        
        strcpy(args[0], strtok(input, " "));
        __pid_t pid = fork();
        char *buffer;
        k = 1;
        
        while(1) {
            buffer = strtok(NULL, " ");
            if (buffer == NULL) {
                args[k] = NULL;
                break;
            }
            strcpy(args[k++], buffer);

        }
        if(pid == 0) {
            
            execvp(args[0], args);
    
        } else {
            wait(&k);
        }
        
    }
}