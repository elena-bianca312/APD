#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define P 3
#define N 16

void *f(void *arg) {
	long id = *(long*) arg;

	/*long start = id * ceil((double) N / P);
	long stop = fmin(N, (id + 1) * ceil((double) N / P));*/

	long start = id * (double) N / P;
	long stop = fmin(N, (id + 1) * (double) N / P);

	printf("T%ld: start = %ld, stop = %ld (%ld iteratii)\n", id, start, stop, stop - start);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[P];
	int r;
	long id;
	void *status;
	long ids[P];

	for (id = 0; id < P; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

		if (r) {
			printf("Eroare la crearea thread-ului %ld\n", id);
			exit(-1);
		}
	}

	for (id = 0; id < P; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %ld\n", id);
			exit(-1);
		}
	}

	return 0;
}
