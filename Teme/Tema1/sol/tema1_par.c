#include <stdlib.h>
#include <stdio.h>
#include "genetic_algorithm_par.h"

int main(int argc, char *argv[]) {
	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;

	// number of threads - eu am adaugat
	int P;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, &P, argc, argv)) {
		return 0;
	}

	// Initialize barrier - to be used by all threads for non-faulty synchronization
	pthread_barrier_t barrier;
	int r = pthread_barrier_init(&barrier, NULL, P);
	if (r) {
		printf("Error barrier\n");
		exit(-1);
	}

	// Declare threads
	pthread_t threads[P];

	// Allocate memory for arrays. To be shared by all threads, so they can all alter different parts of the array
	individual *current_generation = (individual *) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual *) calloc(object_count, sizeof(individual));
	individual *copy = (individual *) calloc(object_count, sizeof(individual));
	individual *tmp = NULL;

	// Array of structs, each thread will pass one struct to its initialiazing function
	struct info args[P];

	// Populate struct, initialize threads with create function
	for (int i = 0; i < P; i++) {
		args[i].id = i;
		args[i].objects = objects;
		args[i].object_count = object_count;
		args[i].sack_capacity = sack_capacity;
		args[i].generations_count = generations_count;
		args[i].P = P;	
		args[i].current_generation = current_generation;
		args[i].next_generation    = next_generation;
		args[i].copy			   = copy; 
		args[i].tmp = tmp;
		args[i].barrier = &barrier;
		pthread_create(&threads[i], NULL, run_genetic_algorithm, &args[i]);
	}

	// Join all threads for program finish
	for (int i = 0; i < P; i++) {
		pthread_join(threads[i], NULL);
	}

	// Free resources
	free(current_generation);
	free(next_generation);

	pthread_barrier_destroy(&barrier);

	free(objects);

	return 0;
}
