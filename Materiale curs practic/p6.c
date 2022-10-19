#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define NUM_THREADS 1
#define ITERATIONS  100000

void *f(void *arg) {
	long id = *(long*) arg;

	long start = id * ceil((double) ITERATIONS / NUM_THREADS);
	long stop = fmin(ITERATIONS, (id + 1) * ceil((double) ITERATIONS / NUM_THREADS));

	int a = 0;
	
	for (int i = start; i < stop; i++) {
		for (int j = 0; j < 20000; j++) {
			a++;
			a *= 2;
		}
	}

	//sleep(2);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
	int r;
	long id;
	void *status;
	long ids[NUM_THREADS];

	for (id = 0; id < NUM_THREADS; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

		if (r) {
			printf("Eroare la crearea thread-ului %ld\n", id);
			exit(-1);
		}
	}

	for (id = 0; id < NUM_THREADS; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %ld\n", id);
			exit(-1);
		}
	}

	return 0;
}
