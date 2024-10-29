#include "syscall.h"
#include "lib.h"


int main(int argc, char** argv) {

    char argErr[] = "Error: missing argument.";
    char cantRemoveErr[] = "Error: could not delete file.";

    if (argc < 2) {
        Write(argErr, strlen(argErr), CONSOLE_OUTPUT);
        Exit(1);
    }

    int ret = 0;
    for (unsigned i = 1; i < argc; i++) {
        if (Remove(argv[i]) < 0) {
            Write(cantRemoveErr, strlen(cantRemoveErr), CONSOLE_OUTPUT);
            ret = 1;
            break;
        }
    }
    return ret;
}