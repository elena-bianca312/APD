#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

pthread_mutex_t mutex;

void *f(void *arg) {
	pthread_mutex_lock(&mutex);
	int* a = (int*) arg;
	*a = *a + 2;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
	int r;
	long id;
	void *status;
	int a = 0;

	pthread_mutex_init(&mutex, NULL);

	for (id = 0; id < NUM_THREADS; id++) {
		r = pthread_create(&threads[id], NULL, f, &a);

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

	pthread_mutex_destroy(&mutex);

	printf("a = %d\n", a);

	return 0;
}
