/*

The cp command syntax for copying files and directories is simple but offers various options for performing advanced actions. Below is the basic form of the command for copying files:

cp [options] [source] [destination]

To copy directories, add the -r (recursive) option:

cp -r [options] [source] [destination]

If the source command argument is a file, the destination can be another file or a directory. If the source is a directory, the destination must also be a directory. 

Nosotros solo implementamos la versión más sencilla (sin recursión), donde tanto source como destination son archivos y ninguno puede ser un directorio.

*/


#include "syscall.h"


int main(int argc, char** argv) {
    if (argc != 3) return -1;
    
    if (Create(argv[2]) == -1) return -1;

    OpenFileId input = Open(argv[1]);

    if (input == -1) return -1;

    OpenFileId output = Open(argv[2]);

    if (output == -1) {
	    Close(input);
        return -1;
    }

    char c;
    while (Read(&c, 1, input)) { Write(&c, 1, output); }

    Close(input); Close(output);

    return 0;
}


