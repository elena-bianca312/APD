#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4

struct my_struct {
	long id;
	float f;
};

void *f(void *arg) {
	//long id = *(long*) arg;
	struct my_struct* args = (struct my_struct*) arg;

	printf("Hello World din thread-ul %ld (f=%f)!\n", args->id, args->f);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
	int r;
	struct my_struct args[NUM_THREADS];
	void *status;
	long id = 0;

	for (id = 0; id < NUM_THREADS; id++) {
		args[id].id = id; 
		args[id].f = 0.25f * (id + 1); 
		r = pthread_create(&threads[id], NULL, f, &args[id]);

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
