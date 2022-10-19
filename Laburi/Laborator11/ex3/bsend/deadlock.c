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

    int buffer_attached_size;
    int *buffer_attached;

    if (rank == 0) {
        for (int i = 0; i < SIZE; i++) {
            num1[i] = 100;
        }
        // Create buffer for sending messages
        buffer_attached_size = MPI_BSEND_OVERHEAD + SIZE * sizeof(int);
        buffer_attached = (int *) malloc(buffer_attached_size);

        // Attach buffer
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);

        // Send message (via created buffer)
        MPI_Bsend(num1, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Receive message
        MPI_Recv(num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    } else {
        for (int i = 0; i < SIZE; i++) {
            num2[i] = 200;
        }
        // Create buffer for sending messages
        buffer_attached_size = MPI_BSEND_OVERHEAD + SIZE * sizeof(int);
        buffer_attached = (int *) malloc(buffer_attached_size);

        // Attach buffer
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);

        // Send message (via created buffer)
        MPI_Bsend(num2, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // Receive message
        MPI_Recv(num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();

    return 0;
}
