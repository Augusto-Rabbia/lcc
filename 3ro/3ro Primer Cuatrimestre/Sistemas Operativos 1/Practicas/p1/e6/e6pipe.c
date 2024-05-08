#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define COM_SIZE 100
#define MAX_ARG_LEN 100
#define MAX_ARGS 10
#define MAX_FILE_PATH 100

void quit(char* err) {
    fputs(err, stderr);
    exit(1);
}

void execute(char** args, int redirectOutput, int output_file_desc) {
    char** a = NULL;
    __pid_t pid = fork();
    if (pid == 0) { // Child process
        dup2(output_file_desc, STDOUT_FILENO);
        execvp(args[0], args);
        quit("Unknown command\n");
    }
    if (pid == -1) quit("fork");
    // Parent process
    int status;
    waitpid(pid, &status, 0);
}

void parse_command(char* com, char** args, int* redirectOutput, char* output_file, int pipefd[2]) {
    char *token = strtok(com, " ");
    int nArgs = 0;
    int hayPipe = 0;
    char pipeArg[MAX_ARG_LEN];

    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            // Redirect output
            token = strtok(NULL, " ");
            *redirectOutput = 1;
            if (token != NULL)
                strncpy(output_file, token, MAX_ARG_LEN);
        } else if (strcmp(token, "|") == 0) {
            // Pipe
            printf("Hay PIPE!\n");
            hayPipe = 1;
            args[nArgs] = NULL;
            close(pipefd[1]);
            execute(args, 0, pipefd[0]);
            *redirectOutput = 2;
            nArgs = 0;
            printf("a\n");
        } else {
            args[nArgs++] = token;
            if (nArgs >= MAX_ARGS) {
                break;
            }
        }
        token = strtok(NULL, " ");
    }
    args[nArgs] = NULL;
}

int main() {
    char command[COM_SIZE];
    char** args = (char**) malloc(sizeof(char*) * MAX_ARGS);
    for (int i = 0; i < MAX_ARGS; i++)
        args[i] = (char*) malloc(sizeof(char) * MAX_ARG_LEN);
    int redirectOutput;
    int output_file_desc;
    char output_file[MAX_FILE_PATH];

    int pipefd[2];
    if (pipe(pipefd) == -1) quit("pipe");

    while(1) {
        redirectOutput = 0;
        output_file_desc = STDOUT_FILENO;
        printf("> ");
        fgets(command, COM_SIZE, stdin);
        command[strcspn(command, "\n")] = '\0';
        if (strcmp(command, "exit") == 0)
            exit(0);
        
        parse_command(command, args, &redirectOutput, output_file, pipefd);
        
        if (redirectOutput) output_file_desc = open(output_file, O_CREAT | O_RDWR);
        else if (redirectOutput == 2) output_file_desc = pipefd[1];

        execute(args, redirectOutput, output_file_desc);
        if (redirectOutput) close(output_file_desc);
    }
    return 0;
}