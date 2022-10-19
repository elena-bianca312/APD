#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

// Big number -> to send large data (for deadlock)
#define SIZE 500000
 
int main (int argc, char *argv[]) {
    int  numtasks, rank;
 
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // The current process ID / Rank.

    int num1[SIZE], num2[SIZE];
 
    if (rank == 0) {
        for (int i = 0; i < SIZE; i++) {
            num1[i] = 100;
        }
        
        // Use MPI_Sendrecv instead of MPI_Send followed by MPI_Recv
        MPI_Sendrecv(num1, SIZE, MPI_INT, 1, 0,
                        num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    } else {
        for (int i = 0; i < SIZE; i++) {
            num2[i] = 200;
        }

        // Use MPI_Sendrecv instead of MPI_Send followed by MPI_Recv
        MPI_Sendrecv(num2, SIZE, MPI_INT, 0, 0,
                        num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();

    return 0;
}
