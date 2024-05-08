#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#define tambuf 4*32

int main(int argc, char **argv)
{
    int pid, npr;
    int i, numdat;
    int buf[tambuf], buf2[tambuf], modoacceso;
    MPI_File dat1;
    MPI_Status info;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);   // n = num procesos
    numdat = 4;
    if (pid == 0) 
        for(i=0; i<npr*numdat; i++) buf[i] = i*i;  // de i = 0 hasta 4*n, buf[i] = i^2
    if (pid == 0){
        modoacceso = (MPI_MODE_CREATE | MPI_MODE_WRONLY);   // Opciones para crear y solo escribir
        MPI_File_open(MPI_COMM_SELF, "dat1", modoacceso, MPI_INFO_NULL, &dat1);  // Master abre el archivo dat1 en modo escritura
        MPI_File_seek(dat1, 0, MPI_SEEK_SET); // Master situamos al inicio del archivo
        MPI_File_write(dat1, buf, npr*numdat, MPI_INT, &info);  //  Master escribe 4*n INTs del buffer en dat1
        MPI_File_close(&dat1);                  // Cerramos el archivo
        printf("\n El master escribi´o %d datos, desde 0 hasta %d \n\n",
        npr*numdat, buf[npr*numdat-1]);          
    }
    sleep(3);
    modoacceso = MPI_MODE_RDONLY;  // Opciones para solo lectura
    MPI_File_open(MPI_COMM_WORLD, "dat1", modoacceso, MPI_INFO_NULL, &dat1);   // Todos los proceso (incluido master), 
                                                                               //acceden a la lectura del archivo

    MPI_File_seek(dat1, pid*numdat*sizeof(int), MPI_SEEK_SET);       // El proceso se ubica en donde corresponde en base a su pid
    MPI_File_read(dat1, buf2, numdat, MPI_INT, &info);              // Se leen 4 INTs a partir de la ubicación
    MPI_File_close(&dat1);            // Se cierra el archivo
    printf(" > %d ha leido %5d %5d %5d %5d \n",
        pid, buf2[0], buf2[1], buf2[2], buf2[3]);
    MPI_Finalize();
    return 0;
}