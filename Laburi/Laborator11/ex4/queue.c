#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

// Struct to define in MPI
typedef struct {
    int size;
    int arr[1000];
} queue;

int main (int argc, char *argv[]) {
    int numtasks, rank;
    int randNr;

    queue q;
    // TODO: declare the types and arrays for offsets and block counts
    MPI_Datatype queue_type, oldtypes[2]; 
    int blockcounts[2];
    MPI_Aint offsets[2];
    MPI_Status status;

    // For "size"
    offsets[0] = offsetof(queue, size);
    oldtypes[0] = MPI_INT;
    blockcounts[0] = 1;

    // For "arr"
    offsets[1] = offsetof(queue, arr);
    oldtypes[1] = MPI_INT;
    blockcounts[1] = 1000;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // TODO: create the MPI data type, using offsets and block counts, and commit the data type
    MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, &queue_type);
    MPI_Type_commit(&queue_type);

    // Each process gets a different random number at each run
    // (based on current time and rank)
    srand((time(0) + rank));
    
    // Initialize queue size
    q.size = 0;

    // First process starts the circle
    if (rank == 0) {
        /* First process starts the circle */
        
        // Generate a random number and add it in queue
        randNr = rand() % 100;
        q.arr[q.size++] = randNr;

        // Send the queue to the next process.
        MPI_Send(&q, 1, queue_type, 1, 0, MPI_COMM_WORLD);

        // Receive the queue from the last process
        MPI_Recv(&q, 1, queue_type, numtasks - 1, 0, MPI_COMM_WORLD, NULL);

    } else if (rank == numtasks - 1) {
        /* Last process closes the circle */
        
        // Receive the queue from the previous process
        MPI_Recv(&q, 1, queue_type, rank - 1, 0, MPI_COMM_WORLD, NULL);
        
        // Generate a random number and add it in queue
        randNr = rand() % 100;
        q.arr[q.size++] = randNr;

        // Send the queue to the first process
        MPI_Send(&q, 1, queue_type, 0, 0, MPI_COMM_WORLD);

    } else {
        /* Middle process */

        // Receive the queue from the previous process
        MPI_Recv(&q, 1, queue_type, rank - 1, 0, MPI_COMM_WORLD, NULL);
        
        // Generate a random number and add it in queue
        randNr = rand() % 100;
        q.arr[q.size++] = randNr;

        // Send the queue to the next process
        MPI_Send(&q, 1, queue_type, rank + 1, 0, MPI_COMM_WORLD);
    }

    // TODO: Process 0 prints the elements from queue
    if (rank == 0) {
        for (int i = 0; i < q.size; i++) {
            printf("%d ", q.arr[i]);
        }
        printf("\n");
    }

    // TODO: free the newly created MPI data type
    MPI_Type_free(&queue_type);

    MPI_Finalize();

    return 0;
}
