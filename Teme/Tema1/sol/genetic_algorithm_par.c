#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "genetic_algorithm_par.h"

// Added P number of threads to be read from cin
int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int *P, int argc, char *argv[])
{
	FILE *fp;

	if (argc < 4) {
		fprintf(stderr, "Usage:\n\t./tema1 in_file generations_count P\n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		return 0;
	}

	if (fscanf(fp, "%d %d", object_count, sack_capacity) < 2) {
		fclose(fp);
		return 0;
	}

	if (*object_count % 10) {
		fclose(fp);
		return 0;
	}

	sack_object *tmp_objects = (sack_object *) calloc(*object_count, sizeof(sack_object));

	for (int i = 0; i < *object_count; ++i) {
		if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
			free(objects);
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);

	*generations_count = (int) strtol(argv[2], NULL, 10);
	
	if (*generations_count == 0) {
		free(tmp_objects);

		return 0;
	}

	*objects = tmp_objects;

	*P = (int) strtol(argv[3], NULL, 10);

	return 1;
}

void print_objects(const sack_object *objects, int object_count)
{
	for (int i = 0; i < object_count; ++i) {
		printf("%d %d\n", objects[i].weight, objects[i].profit);
	}
}

void print_generation(const individual *generation, int limit)
{
	for (int i = 0; i < limit; ++i) {
		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			printf("%d ", generation[i].chromosomes[j]);
		}

		printf("\n%d - %d\n", i, generation[i].fitness);
	}
}

void print_best_fitness(const individual *generation)
{
	printf("%d\n", generation[0].fitness);
}

// Parallel function, modified outer for from start to end read from args parameters
void compute_fitness_function(struct info *args)
{

	int weight;
	int profit;
	
	for (int i = args->start; i < args->end; ++i) {
		
		weight = 0;
		profit = 0;

		for (int j = 0; j < args->current_generation[i].chromosome_length; ++j) {
	
			if (args->current_generation[i].chromosomes[j]) {
				weight += args->objects[j].weight;
				profit += args->objects[j].profit;
			}
		}

		args->current_generation[i].fitness = (weight <= args->sack_capacity) ? profit : 0;

	}
}

int cmpfunc(const void *a, const void *b)
{
	individual *first = (individual *) a;
	individual *second = (individual *) b;

	int res = second->fitness - first->fitness; // decreasing by fitness
	if (res == 0) {

		res = first->set_chromosomes - second->set_chromosomes;

		if (res == 0) {
			return second->index - first->index;
		}
	}

	return res;
}

void mutate_bit_string_1(individual *ind, int generation_index)
{
		int i, mutation_size;
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	if (ind->index % 2 == 0) {
		// for even-indexed individuals, mutate the first 40% chromosomes by a given step
		mutation_size = ind->chromosome_length * 4 / 10;
		for (i = 0; i < mutation_size; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	} else {
		// for odd-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(individual *ind, int generation_index)
{
	int step = 1 + generation_index % (ind->chromosome_length - 2);

	// mutate all chromosomes by a given step
	for (int i = 0; i < ind->chromosome_length; i += step) {
		ind->chromosomes[i] = 1 - ind->chromosomes[i];
	}
}

void crossover(individual *parent1, individual *child1, int generation_index)
{
	individual *parent2 = parent1 + 1;
	individual *child2 = child1 + 1;
	int count = 1 + generation_index % parent1->chromosome_length;

	memcpy(child1->chromosomes, parent1->chromosomes, count * sizeof(int));
	memcpy(child1->chromosomes + count, parent2->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));

	memcpy(child2->chromosomes, parent2->chromosomes, count * sizeof(int));
	memcpy(child2->chromosomes + count, parent1->chromosomes + count, (parent1->chromosome_length - count) * sizeof(int));
}

void calculate_set_chromosomes(struct info *args) {

	int number;
	for (int i = args->start; i < args->end; i++) {
		number = 0;
		for (int j = 0; j < args->current_generation[i].chromosome_length; j++) {
			number += args->current_generation[i].chromosomes[j];
		}
		args->current_generation[i].set_chromosomes = number;
	}
}

void copy_individual(individual *from, individual *to)
{
	memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
	to->set_chromosomes = from->set_chromosomes;
}

// Parallel function, modified for. Each thread frees a part of the array
void free_generation(struct info *args, individual *generation)
{
	int i;

	for (i = args->start; i < args->end; ++i) {
		free(generation[i].chromosomes);
		generation[i].chromosomes = NULL;
		generation[i].fitness = 0;
	}
}

// Min helper function
int min(int a, int b) {

	return a < b ? a : b;
}

// Merge function - adds two sorted halves of an array to a final array by comparisons
void merge(struct info *args, int start, int mid, int end)
{
    int iA = start, iB = mid;

    for (int i = start; i < end; i++) {

		if (end == iB || (iA < mid && cmpfunc(&args->current_generation[iA], &args->current_generation[iB]) <= 0)) {
            args->copy[i] = args->current_generation[iA];
            iA++;
        } else {
            args->copy[i] = args->current_generation[iB];
            iB++;    
        }

    }

}

/* Main merge function - controls the width of the elements grouped
					   - calculates start and end for each thread */
void mergeSort(struct info *args)
{
   
    for (int width = 1; width < args->object_count; width <<= 1)
    {
   
		int groups = ceil((double)args->object_count / (2 * width));

		args->start = args->id * (double)groups / args->P;
		args->end = min(groups, (args->id + 1) * (double)groups / args->P);

		for (int i = args->start * width * 2; i < args->end * width * 2; i += width << 1)
		{

			merge(args, i, min(i + width, args->object_count),
				min(i + 2 * width, args->object_count));
		
		}

		// Copy elements from copy to current_generation to keep track of the changes
		pthread_barrier_wait(args->barrier);
		args->tmp = args->current_generation;
		args->current_generation = args->copy;
		args->copy = args->tmp;

		for (int i = args->start; i < args->end; ++i) {
			args->copy[i].index = i;
		}

		// Wait for all threads to finish before iterating next level
		pthread_barrier_wait(args->barrier);
	}
}


void *run_genetic_algorithm(void *arg)
{

	struct info *args = (struct info *) arg;

	args->start = args->id * (double) args->object_count / args->P;
	args->end   = min((args->id + 1) * (double) args->object_count / args->P, args->object_count);

	// set initial generation (composed of object_count individuals with a single item in the sack)
	for (int i = args->start; i < args->end; ++i) {
		args->current_generation[i].fitness = 0;
		args->current_generation[i].chromosomes = (int*) calloc(args->object_count, sizeof(int));
		args->current_generation[i].chromosomes[i] = 1;
		args->current_generation[i].index = i;
		args->current_generation[i].chromosome_length = args->object_count;
		args->current_generation[i].set_chromosomes = 0;

		args->next_generation[i].fitness = 0;
		args->next_generation[i].chromosomes = (int*) calloc(args->object_count, sizeof(int));
		args->next_generation[i].index = i;
		args->next_generation[i].chromosome_length = args->object_count;
		args->next_generation[i].set_chromosomes = 0;

		args->copy[i].fitness = 0;
		args->copy[i].chromosomes = (int*) calloc(args->object_count, sizeof(int));
		args->copy[i].index = i;
		args->copy[i].chromosome_length = args->object_count;
		args->copy[i].set_chromosomes = 0;
	}


	pthread_barrier_wait(args->barrier);

	// iterate for each generation
	for (int k = 0; k < args->generations_count; ++k) {
		args->cursor = 0;

		// Calculate start and end for each thread by object count
		args->start = args->id * (double) args->object_count / args->P;
		args->end   = min((args->id + 1) * (double) args->object_count / args->P, args->object_count);

		// compute fitness and sort by it
		compute_fitness_function(args);
		pthread_barrier_wait(args->barrier);

		// Sort threads by fitness (decreasing)
		mergeSort(args);
		pthread_barrier_wait(args->barrier);

		// keep first 30% children (elite children selection)
		args->count = args->object_count * 3 / 10;
		args->start = args->id * (double) args->count / args->P;
		args->end   = min((args->id + 1) * (double) args->count / args->P, args->count);
		for (int i = args->start; i < args->end; ++i) {
			copy_individual(args->current_generation + i, args->next_generation + i);
		}
		args->cursor = args->count;
		pthread_barrier_wait(args->barrier);

		// mutate first 20% children with the first version of bit string mutation
		args->count = args->object_count * 2 / 10;
		args->start = args->id * (double) args->count / args->P;
		args->end   = min((args->id + 1) * (double) args->count / args->P, args->count);
		for (int i = args->start; i < args->end; ++i) {
			copy_individual(args->current_generation + i, args->next_generation + args->cursor + i);
			mutate_bit_string_1(args->next_generation + args->cursor + i, k);
		}
		args->cursor += args->count;
		pthread_barrier_wait(args->barrier);

		// mutate next 20% children with the second version of bit string mutation
		args->count = args->object_count * 2 / 10;
		for (int i = args->start; i < args->end; ++i) {
			copy_individual(args->current_generation + i + args->count, args->next_generation + args->cursor + i);
			mutate_bit_string_2(args->next_generation + args->cursor + i, k);
		}
		args->cursor += args->count;
		pthread_barrier_wait(args->barrier);

		// crossover first 30% parents with one-point crossover
		// (if there is an odd number of parents, the last one is kept as such)
		args->count = args->object_count * 3 / 10;

		if (args->count % 2 == 1) {
			copy_individual(args->current_generation + args->object_count - 1, args->next_generation + args->cursor + args->count - 1);
			args->count--;
		}
		pthread_barrier_wait(args->barrier);

		args->start = args->id * (double) args->count / args->P;
		args->end   = min((args->id + 1) * (double) args->count / args->P, args->count);

		if (args->start % 2 == 1) {
			args->start--;
		}

		for (int i = args->start; i < args->end; i += 2) {
			crossover(args->current_generation + i, args->next_generation + args->cursor + i, k);
		}
		pthread_barrier_wait(args->barrier);

		args->start = args->id * (double) args->object_count / args->P;
		args->end   = min((args->id + 1) * (double) args->object_count / args->P, args->object_count);

		// Calculate number of chromosomes for each individual so we can avoid that for recursive sorting
		calculate_set_chromosomes(args);
		pthread_barrier_wait(args->barrier);

		// switch to new generation
		args->tmp = args->current_generation;
		args->current_generation = args->next_generation;
		args->next_generation = args->tmp;

		for (int i = args->start; i < args->end; ++i) {
			args->current_generation[i].index = i;
		}
		pthread_barrier_wait(args->barrier);

		// Print best fitness - only one thread to avoid copies
		if (k % 5 == 0 && args->id == 0) {
			print_best_fitness(args->current_generation);
		}

		pthread_barrier_wait(args->barrier);
	}

	args->start = args->id * (double) args->object_count / args->P;
	args->end   = min((args->id + 1) * (double) args->object_count / args->P, args->object_count);

	compute_fitness_function(args);
	pthread_barrier_wait(args->barrier);

	mergeSort(args);
	pthread_barrier_wait(args->barrier);

	if (args->id == 0) {
		print_best_fitness(args->current_generation);
	}

	args->start = args->id * (double) args->current_generation->chromosome_length / args->P;
	args->end   = min((args->id + 1) * (double) args->current_generation->chromosome_length / args->P, args->current_generation->chromosome_length);
	
	
	// Free resources
	free_generation(args, args->current_generation);
	free_generation(args, args->next_generation);

	pthread_barrier_wait(args->barrier);

	pthread_exit(NULL);
}
