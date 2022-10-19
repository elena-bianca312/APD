#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int id;
	int no_threads;
    int* vector;
    int vector_size;
    int* sum;
}thread_struct;


void *thread_func(void *arg) {
    thread_struct t = *(thread_struct*) arg;
    t.sum[0] = 0;
    int start = t.id * (double) t.vector_size / t.no_threads;
	int end = t.vector_size;
    if (end > (t.id + 1) * (double)t.vector_size / t.no_threads) {
		end = (t.id + 1) * (double)t.vector_size / t.no_threads;
	}

    for (int i = start; i < end; i++) {
        t.sum[0] += t.vector[i];
    }
        printf("%d\n", t.sum[0]);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int NUM_THREADS = atoi(argv[2]);
    pthread_t threads[NUM_THREADS];
    int r;
    long id;
    void *status;
    long ids[NUM_THREADS];
    thread_struct t[NUM_THREADS];

    int vector[atoi(argv[1])];

    for (int i = 0; i < atoi(argv[1]) - 1; i++) {
        vector[i] = atoi(argv[i + 3]);
    }

    for (id = 0; id < NUM_THREADS; id++) {
        t[id].id = id;
        t[id].no_threads = NUM_THREADS;
        t[id].vector = vector;
        t[id].vector_size = atoi(argv[1]) - 1;
        t[id].sum = malloc(sizeof(int));

        r = pthread_create(&threads[id], NULL, thread_func, &t[id]);

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    int sum = 0;
    for (id = 0; id < NUM_THREADS; id++) {
        r = pthread_join(threads[id], &status);
        sum += t[id].sum[0];
        printf("%d\n", sum);
        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    printf("%d\n", ((atoi(argv[1]) * (atoi(argv[1]) + 1)) / 2 - sum));

    pthread_exit(NULL);
}