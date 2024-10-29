/*
This command displays the contents of one or more files without having to open the file for editing.

To use the cat command, follow the format:

cat [options] filename(s)

[options] – This lets you issue additional instructions to the cat command. For example, to display the contents of a file with each line numbered, use the –n option:

cat –n filename

filename(s) – Specify the name of the file (or files) that you want to display. If you use more than one filename, each file will be displayed.

Implementamos una versión simplificada que solo imprime el contenido de un archivo en la consola y no toma opciones adicionales.

*/


#include "syscall.h"


int main(int argc, char** argv) {

    if (argc != 2) return -1;

    OpenFileId fd = Open(argv[1]);

    if (fd == -1) return -1;

    char c;
    while (Read(&c, 1, fd)) { Write(&c, 1, CONSOLE_OUTPUT); }

    Close(fd);

    return 0;
}

