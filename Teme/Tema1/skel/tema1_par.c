#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "genetic_algorithm_par.h"

int main(int argc, char *argv[]) {

	// -----------------------------------------------------------------------
	// ------------ Declare variables to store input values ------------------
	// -----------------------------------------------------------------------

	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;

	// number of threads
	int P;


	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, &P, argc, argv)) {
		return 0;
	}

    // printf("threads: %d\n", P);

	// --------------------------------------------------------------------------
	// ------------ Declare structs for thread function calling -----------------
	// --------------------------------------------------------------------------

	// create threads and structs
    int i, r;
	pthread_t threads[P];				// declare threads
    struct info args;					// struct for basic details, the same for all threads
	struct tid tids[P];					// main structure to be passed to function, contains thread id etc

	r = pthread_mutex_init(&mutex, NULL);
	if (r) {
		printf("Error mutex\n");
		exit(-1);
	}

	r = pthread_barrier_init(&barrier, NULL, P);
	if (r) {
		printf("Error barrier\n");
		exit(-1);
	}

	individual *current_generation = (individual *) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual *) calloc(object_count, sizeof(individual));

	args.objects = objects;
	args.object_count = object_count;
	args.sack_capacity = sack_capacity;
	args.generations_count = generations_count;
	args.P = P;

	for (int i = 0; i < args.P; i++) {
        tids[i].id = i;
		tids[i].args = &args;
		tids[i].current_generation = current_generation;
		tids[i].next_generation = next_generation;
		tids[i].tmp = NULL;
		pthread_create(&threads[i], NULL, run_genetic_algorithm, &tids[i]);
	}

	// run_genetic_algorithm(objects, object_count, generations_count, sack_capacity);

    // join threads 
    for (i = 0; i < args.P; i++) {
		pthread_join(threads[i], NULL);
	}

	free(args.objects);

	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

	return 0;
}
