#include <mpi.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>
 
using namespace std;

int main (int argc, char *argv[]) {
    int numtasks, rank;

    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numtasks < 2 || numtasks > 12) {
        if (rank == 0) {
            cout << "Number of processes should be between [2, 12].";
        }
        MPI_Finalize();
        return 0;
    }

    int i, j;

    int value = rank * 2;
    int recvValue;

    // Task 1

    for (i = 0; i < numtasks; i++) {
        if (i % 2 == 0) {
            // even rank
            if (rank % 2 == 0) {
                // Evens send to right neighbour + receive from right neighbour
                if (rank < numtasks - 1) {
                    MPI_Send(&value, 1, MPI_INT, rank + 1, 2, MPI_COMM_WORLD);
                    MPI_Recv(&recvValue, 1, MPI_INT, rank + 1, 2, MPI_COMM_WORLD, &status);
                    if (recvValue > value)
                        value = recvValue;
                }
            } 
            // odd rank
            else {
                // Odds send to left neighbour + receive from left neighbour
                if (rank > 0) {
                    MPI_Send(&value, 1, MPI_INT, rank - 1, 2, MPI_COMM_WORLD);
                    MPI_Recv(&recvValue, 1, MPI_INT, rank - 1, 2, MPI_COMM_WORLD, &status);
                    if (recvValue < value)
                        value = recvValue;
                }
            }
        } else {
            // odd rank
            if (rank % 2 == 1) {
                // Odds send to right neighbour + receive from right neighbour
                if (rank < numtasks - 1) {
                    MPI_Send(&value, 1, MPI_INT, rank + 1, 2, MPI_COMM_WORLD);
                    MPI_Recv(&recvValue, 1, MPI_INT, rank + 1, 2, MPI_COMM_WORLD, &status);
                    if (recvValue > value)
                        value = recvValue;
                }
            } 
            // even rank
            else {
                // Evens send to left neighbour + receive from left neighbour
                if (rank > 0) {
                    MPI_Send(&value, 1, MPI_INT, rank - 1, 2, MPI_COMM_WORLD);
                    MPI_Recv(&recvValue, 1, MPI_INT, rank - 1, 2, MPI_COMM_WORLD, &status);
                    if (recvValue < value)
                        value = recvValue;
                }
            }
        }
    }

    cout << rank << " - " << value << endl;

    // Task 2

    // Process numtasks - 1 has to receive all values

    vector<int> values;

    // Receive values from previous process
    // Add it to values vector (different for each process)
    for (i = 0; i < rank; i++) {
        MPI_Recv(&recvValue, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
        values.push_back(recvValue);
    }

    // If we have a right neighbour, we will send the vector of values to the next process
    if (rank < numtasks - 1) {
        for (i = 0; i < values.size(); i++) {
            MPI_Send(&values[i], 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
        }
        // Send my own value
        MPI_Send(&value, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
    }

    // The process with the biggest rank will print the numbers sorted in decreasing order
    if (rank == numtasks - 1) {
        for (i = 0; i < values.size(); i++) {
            cout << values[i] << " ";
        }
        cout << value << endl;
    }

    MPI_Finalize();

    return 0;
}
