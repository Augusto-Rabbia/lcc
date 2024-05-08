#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

// Elegimos en esta caso como valor privado el rango del proceso

int main(int argc, char **argv) {
    int rank, whileval = 1, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Id del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número de procesos
    
    int sum = rank; // La suma se hará con los rangos de los diferentes procesos
    int recvd;   // Para recibir enteros de otros procesos
    
    // Se envía el valor privado al proceso con el pid "siguiente" en el circulo.
    MPI_Send(&rank, 1, MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD);
    // Se recibe (bloqueante) el valor privado del proceso "anterior" en el circulo.
    MPI_Recv(&recvd, 1, MPI_INT, (rank+size-1) % size, 0, MPI_COMM_WORLD, &status); 
    sum += recvd; 
    

    while (++whileval < size) {
        // Se envía el valor privado al proceso con el pid "siguiente" en el circulo.
        MPI_Send(&recvd, 1, MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD);
        // Se recibe (bloqueante) el valor privado del proceso "anterior" en el circulo.
        MPI_Recv(&recvd, 1, MPI_INT, (rank-1) % size, 0, MPI_COMM_WORLD, &status);
        sum += recvd; 
    }

    printf("process: %d, finished with: %d\n", rank, sum);

    MPI_Finalize();
    return 0;
}