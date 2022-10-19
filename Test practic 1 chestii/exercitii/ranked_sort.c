#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int id;
	int no_threads;
	pthread_barrier_t *barrier;

    int* vector;
    int vector_size;
    int* result;
}thread_struct;


void *thread_func(void *arg) {
    thread_struct t = *(thread_struct*) arg;

    int start = t.id * (double) t.vector_size / t.no_threads;
	int end = t.vector_size;
    if (end > (t.id + 1) * (double)t.vector_size / t.no_threads) {
		end = (t.id + 1) * (double)t.vector_size / t.no_threads;
	}

    // Rank sort
    for (int j = start; j < end; j++) {
        int currentItem = t.vector[j];
        int currentPosition = 0;
        for (int i = 0; i < t.vector_size; i++) {
            if (currentItem > t.vector[i]) {
                currentPosition++;
            }
        }
        t.result[currentPosition] = currentItem;
    }

	pthread_barrier_wait(t.barrier);

    if (t.id == 0) {
        for (int i = 0; i < t.vector_size; i++) {
            printf("%d ", t.result[i]);
        }
        printf("\n");
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int NUM_THREADS = atoi(argv[2]);
    pthread_t threads[NUM_THREADS];
    int r;
    long id;
    void *status;
    long ids[NUM_THREADS];
    pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    thread_struct t[NUM_THREADS];

    int vector[atoi(argv[1])], result[atoi(argv[1])];

    for (int i = 0; i < atoi(argv[1]); i++) {
        vector[i] = atoi(argv[1]) - i + 5;
    }

    for (id = 0; id < NUM_THREADS; id++) {
        t[id].id = id;
        t[id].barrier = &barrier;
        t[id].no_threads = NUM_THREADS;
        t[id].vector = vector;
        t[id].vector_size = atoi(argv[1]);
        t[id].result = result;

        r = pthread_create(&threads[id], NULL, thread_func, &t[id]);

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

    pthread_exit(NULL);
}