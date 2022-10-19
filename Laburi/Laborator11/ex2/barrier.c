#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

int main (int argc, char *argv[]) {
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if (rank == 0) {
        printf("%ld: 1\n", time(0));
        // fflush(stdout); doesn't help - see Readme.md for details
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 1) {
        printf("%ld: 2\n", time(0));
    }

    MPI_Barrier(MPI_COMM_WORLD);    

    if (rank == 2) {
        printf("%ld: 3\n", time(0));
    }

    MPI_Finalize();

    return 0;
}
