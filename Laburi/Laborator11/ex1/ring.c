#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

int main (int argc, char *argv[]) {
    int numtasks, rank;

    MPI_Request request;
    MPI_Status status;
    int flag;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int recv_num;

    // First process starts the circle
    if (rank == 0) {
        /* First process starts the circle */

        // Generate a random number
        srand(time(0));
        int randnr = rand() % 100;

        // Print number
        printf("R: %d;  Nr: %d\n", rank, randnr);

        // Send the number to the next process
        MPI_Isend(&randnr, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        // Wait for send to complete
        MPI_Test(&request, &flag, &status);
        if (!flag) {
            MPI_Wait(&request, &status);
        }


        // Receive the number from the last process
        MPI_Irecv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD, &request);
        
        // Wait for receive to complete
        MPI_Test(&request, &flag, &status);
        if (!flag) {
            MPI_Wait(&request, &status);
        }

        // Print number
        printf("R: %d;  Nr: %d\n", rank, recv_num);

    } else if (rank == numtasks - 1) {
        /* Last process close the circle */

        // Receive the number from the previous process
        MPI_Irecv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &request);
        
        // Wait for receive to complete
        MPI_Test(&request, &flag, &status);
        if (!flag) {
            MPI_Wait(&request, &status);
        }
        
        // Increment the number by 2
        recv_num += 2;

        // Print number
        printf("R: %d;  Nr: %d\n", rank, recv_num);


        // Send the number to the first process
        MPI_Isend(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        
        // Wait for send to complete
        MPI_Test(&request, &flag, &status);
        if (!flag) {
            MPI_Wait(&request, &status);
        }

    } else {
        /* Middle process */
        
        // Receive the number from the previous process
        MPI_Irecv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &request);
        
        // Wait for receive to complete
        MPI_Test(&request, &flag, &status);
        if (!flag) {
            MPI_Wait(&request, &status);
        }

        // Increment the number by 2
        recv_num += 2;

        // Print number
        printf("R: %d;  Nr: %d\n", rank, recv_num);


        // Send the number to the next process
        MPI_Isend(&recv_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &request);
        
        // Wait for send to complete
        MPI_Test(&request, &flag, &status);
        if (!flag) {
            MPI_Wait(&request, &status);
        }
    }

    MPI_Finalize();

    return 0;
}
