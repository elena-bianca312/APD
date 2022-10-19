#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int  numtasks, rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int recv_num;

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        int randNumber = rand() % 100;
        printf("Process with rank %d has number %d\n", rank, randNumber);
        // Send the number to the next process.
        MPI_Send(&randNumber, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(&randNumber, 1, MPI_INT, numtasks - 1, 1, MPI_COMM_WORLD, &status);

    } else if (rank == numtasks - 1) {
        // Last process close the circle.
        // Receives the number from the previous process.
        int receivedNumber;
        MPI_Recv(&receivedNumber, 1, MPI_INT, numtasks - 2, 1, MPI_COMM_WORLD, &status);
        // Increments the number.
        receivedNumber += 2;
        printf("Process with rank %d has number %d\n", rank, receivedNumber);
        // Sends the number to the first process.
        MPI_Send(&receivedNumber, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

    } else {
        // Middle process.
        // Receives the number from the previous process.
        int receivedNumber;
        MPI_Recv(&receivedNumber, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
        // Increments the number.
        receivedNumber += 2;
        printf("Process with rank %d has number %d\n", rank, receivedNumber);
        // Sends the number to the next process.
        MPI_Send(&receivedNumber, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();

}

