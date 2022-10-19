#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "genetic_algorithm_par.h"

// int read_input(void *arg) {

// 	struct info* args = (struct info*) arg;

// 	FILE *fp;

// 	if (args->argc < 4) {
// 		fprintf(stderr, "Usage:\n\t./tema1_par in_file generations_count P\n");
// 		return 0;
// 	}

// 	fp = fopen(args->argv[1], "r");
// 	if (fp == NULL) {
// 		return 0;
// 	}

// 	if (fscanf(fp, "%d %d", args->object_count, args->sack_capacity) < 2) {
// 		fclose(fp);
// 		return 0;
// 	}

// 	if (*(&args->object_count) % 10) {
// 		fclose(fp);
// 		return 0;
// 	}

// 	sack_object *tmp_objects = (sack_object *) calloc(*(&args->object_count), sizeof(sack_object));

// 	for (int i = 0; i < *(&args->object_count); ++i) {
// 		if (fscanf(fp, "%d %d", &tmp_objects[i].profit, &tmp_objects[i].weight) < 2) {
// 			free(args->objects);
// 			fclose(fp);
// 			return 0;
// 		}
// 	}

// 	fclose(fp);

// 	*(&args->generations_count) = (int) strtol(args->argv[2], NULL, 10);
	
// 	if (*(&args->generations_count) == 0) {
// 		free(tmp_objects);

// 		return 0;
// 	}

// 	*(&args->objects) = tmp_objects;

//     *(&args->P) = (int) strtol(args->argv[3], NULL, 10);

// 	return 1;
// }


int read_input(sack_object **objects, int *object_count, int *sack_capacity, int *generations_count, int *P, int argc, char *argv[])
{
	FILE *fp;

	if (argc < 4) {
		fprintf(stderr, "Usage:\n\t./tema1_par in_file generations_count P\n");
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

void compute_fitness_function(const sack_object *objects, individual *generation, int start, int end, int sack_capacity)
{
	int weight;
	int profit;

	for (int i = start; i < end; ++i) {
		weight = 0;
		profit = 0;

		for (int j = 0; j < generation[i].chromosome_length; ++j) {
			if (generation[i].chromosomes[j]) {
				weight += objects[j].weight;
				profit += objects[j].profit;
			}
		}

		generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
	}
}

int cmpfunc(const void *a, const void *b)
{
	int i;
	individual *first = (individual *) a;
	individual *second = (individual *) b;

	int res = second->fitness - first->fitness; // decreasing by fitness
	if (res == 0) {
		int first_count = 0, second_count = 0;

		for (i = 0; i < first->chromosome_length && i < second->chromosome_length; ++i) {
			first_count += first->chromosomes[i];
			second_count += second->chromosomes[i];
		}

		res = first_count - second_count; // increasing by number of objects in the sack
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
		// for even-indexed individuals, mutate the last 80% chromosomes by a given step
		mutation_size = ind->chromosome_length * 8 / 10;
		for (i = ind->chromosome_length - mutation_size; i < ind->chromosome_length; i += step) {
			ind->chromosomes[i] = 1 - ind->chromosomes[i];
		}
	}
}

void mutate_bit_string_2(const individual *ind, int generation_index)
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

void copy_individual(individual *from, individual *to)
{
	// De ce nu e bun memcpy?
	// memcpy(to->chromosomes, from->chromosomes, from->chromosome_length * sizeof(int));
	to->chromosomes = from->chromosomes;
}

void free_generation(individual *generation)
{
	int i;

	for (i = 0; i < generation->chromosome_length; ++i) {
		free(generation[i].chromosomes);
		generation[i].chromosomes = NULL;
		generation[i].fitness = 0;
	}
}

int min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

// void run_genetic_algorithm(const sack_object *objects, int object_count, int generations_count, int sack_capacity)
void *run_genetic_algorithm(void *arg) {

	struct tid *tids = (struct tid*) arg;
	int start, end, start_count, end_count;

	start = tids->id * (double) tids->args->object_count / tids->args->P;
	end = min((tids->id + 1) * (double) tids->args->object_count / tids->args->P, tids->args->object_count);

	int count, cursor;

	// printf("thread:%d	gen add:%d\n", tids->id, tids->current_generation);

	// set initial generation (composed of object_count individuals with a single item in the sack)
	for (int i = start; i < end; ++i) {
		tids->current_generation[i].fitness = 0;
		tids->current_generation[i].chromosomes = (int *) calloc(tids->args->object_count, sizeof(int));
		tids->current_generation[i].chromosomes[i] = 1;
		tids->current_generation[i].index = i;
		tids->current_generation[i].chromosome_length = tids->args->object_count;

		tids->next_generation[i].fitness = 0;
		tids->next_generation[i].chromosomes = (int *) calloc(tids->args->object_count, sizeof(int));
		tids->next_generation[i].index = i;
		tids->next_generation[i].chromosome_length = tids->args->object_count;
	}

	// Wait for all threads to finish
	pthread_barrier_wait(&barrier);

	// a mers, a schimbat in memoria comuna
	// if(tids->id == 1) {
	// 	tids->args->objects[0].profit = 20;
	// }

	// iterate for each generation
	for (int k = 0; k < tids->args->generations_count; ++k) {
		cursor = 0;

		// compute fitness and sort by it
		compute_fitness_function(tids->args->objects, tids->current_generation, start, end, tids->args->sack_capacity);
		pthread_barrier_wait(&barrier);
		qsort(tids->current_generation, tids->args->object_count, sizeof(individual), cmpfunc);
		pthread_barrier_wait(&barrier);

		// pthread_mutex_lock(&mutex);
		// printf("k:%d	thread:%d\n", k, tids->id);
		// for(int t = 0; t < tids->args->object_count; t++) {
		// 	printf("%d\n", tids->current_generation[t].fitness);
		// }
		// pthread_mutex_unlock(&mutex);

		// keep first 30% children (elite children selection)
		count = tids->args->object_count * 3 / 10;

		start_count = tids->id * (double) count / tids->args->P;
		end_count = min((tids->id + 1) * (double) count / tids->args->P, count);

		// pthread_mutex_lock(&mutex);
		// printf("%d %d\n", start_count, end_count);

		for (int i = start_count; i < end_count; ++i) {
			// printf("thread:%d	i:%d\n", tids->id, i);
			copy_individual(tids->current_generation + i, tids->next_generation + i);
		}
		pthread_barrier_wait(&barrier);
		cursor = count;
		pthread_barrier_wait(&barrier);
		// pthread_mutex_unlock(&mutex);


		// mutate first 20% children with the first version of bit string mutation
		count = tids->args->object_count * 2 / 10;

		start_count = tids->id * (double) count / tids->args->P;
		end_count = min((tids->id + 1) * (double) count / tids->args->P, count);

		printf("first\n");

		for (int i = 0; i < count; ++i) {
			printf("thread:%d	i:%d\n", tids->id, i);
			copy_individual(tids->current_generation + i, tids->next_generation + cursor + i);
			mutate_bit_string_1(tids->next_generation + cursor + i, k);
		}
		pthread_barrier_wait(&barrier);
		cursor += count;
		pthread_barrier_wait(&barrier);

		printf("second\n");

		// mutate next 20% children with the second version of bit string mutation
		count = tids->args->object_count * 2 / 10;
		for (int i = start_count; i < end_count; ++i) {
			printf("thread:%d	i:%d\n", tids->id, i);
			copy_individual(tids->current_generation + i + count, tids->next_generation + cursor + i);
			mutate_bit_string_2(tids->next_generation + cursor + i, k);
		}
		pthread_barrier_wait(&barrier);
		cursor += count;

		// // crossover first 30% parents with one-point crossover
		// // (if there is an odd number of parents, the last one is kept as such)
		// count = object_count * 3 / 10;

		// if (count % 2 == 1) {
		// 	copy_individual(current_generation + object_count - 1, next_generation + cursor + count - 1);
		// 	count--;
		// }

		// for (int i = 0; i < count; i += 2) {
		// 	crossover(current_generation + i, next_generation + cursor + i, k);
		// }

		// // switch to new generation
		// tmp = current_generation;
		// current_generation = next_generation;
		// next_generation = tmp;

		// for (int i = 0; i < object_count; ++i) {
		// 	current_generation[i].index = i;
		// }

		pthread_mutex_lock(&mutex);
		if (k % 5 == 0) {
			// printf("k:%d	thread:%d\n", k, tids->id);
			// for(int t = 0; t < tids->args->object_count; t++) {
			// 	printf("%d\n", current_generation[t].fitness);
			// }
			print_best_fitness(tids->current_generation);
		}
		pthread_mutex_unlock(&mutex);
	}

	compute_fitness_function(tids->args->objects, tids->current_generation, start, end, tids->args->sack_capacity);
	qsort(tids->current_generation, tids->args->object_count, sizeof(individual), cmpfunc);
	print_best_fitness(tids->current_generation);

	// free resources for old generation
	// free_generation(tids->current_generation);
	// free_generation(tids->next_generation);

	// // free resources
	// free(tids->current_generation);
	// free(tids->next_generation);

	pthread_exit(NULL);
}