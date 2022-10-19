#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 1000
#define MASTER 0

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i ", displayWidth, v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}
 
int main(int argc, char * argv[]) {
	int rank, i, j;
	int nProcesses;
	MPI_Init(&argc, &argv);
	int pos[N];
	int sorted = 0;
	int *v = (int*)malloc(sizeof(int)*N);
	int *vQSort = (int*)malloc(sizeof(int)*N);

	for (i = 0; i < N; i++)
		pos[i] = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

    if (rank == MASTER) {
        // generate random vector
		for (int i = 0; i < N; i++) {
			int p = rand() % 100;
			if (p % 2 == 0) {
				v[i] = 2 * N - i;
			}
			else {
				v[i] = N - i;
			}
		}
    }

    // send the vector to all processes
	int MULTI  = N / nProcesses;
	int* vSorted = (int*)malloc(sizeof(int) * N);
	
	MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank == 0) {
		// DO NOT MODIFY
		displayVector(v);

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		// sort the vector v
		for (int i = 0; i < MULTI; i++) {
			for (int j = 0; j < N; j++) {
				if (v[rank * MULTI + i] > v[j]) {
					pos[rank * MULTI + i]++;
				}
			}
		}
		
        // recv the new pozitions
		MPI_Gather(&pos[rank * MULTI], MULTI, MPI_INT, pos, MULTI, MPI_INT, 0, MPI_COMM_WORLD);

		for (int i = 0; i < N; i++) {
			vSorted[pos[i]] = v[i];
		}

		displayVector(vSorted);
		compareVectors(vSorted, vQSort);
	} else {
		
        // compute the positions
		for (int i = 0; i < MULTI; i++) {
			for (int j = 0; j < N; j++) {
				if (v[rank * MULTI + i] > v[j]) {
					pos[rank * MULTI + i]++;
				}
			}
		}
        // send the new positions to process MASTER
		MPI_Gather(&pos[rank * MULTI], MULTI, MPI_INT, NULL, MULTI, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
