#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct _thread_args
{
	int id;
	int nr_threads;
	int *vect;
	int N;
	int start;
	int end;
	int sum;
} thread_args;


int min(int a, int b) {
    return a <= b ? a : b;
}

void* thread_function(void *arg){
	thread_args *arguments = (thread_args *)arg;
	//int sum = 0;

 	arguments->start = arguments->id * ((double) (arguments->N - 1) / arguments->nr_threads);
 	arguments->end = min(arguments->N, (arguments->id + 1) * ((double) (arguments->N - 1) / arguments->nr_threads));

 	for(int i = arguments->start; i < arguments->end; i++){
 		arguments->sum += arguments->vect[i];
 	}

}

int main(int argc, char const *argv[])
{
	int numbers = atoi(argv[1]);
	int no_threads = atoi(argv[2]);

	pthread_t *threads = malloc(no_threads * sizeof(pthread_t));
	thread_args *arguments = malloc(no_threads * sizeof(thread_args));

	int *array = malloc(numbers * sizeof(int));

	int suma_completa = 0;

	for(int i = 1; i <= numbers; i++){
		suma_completa += i;
		printf("%d ", i);
	}

	for(int i = 0; i < numbers - 1; i++){
		scanf("%d", &array[i]);
	}

	for(int i = 0; i < no_threads; i++){
		arguments[i].id = i;
		arguments[i].nr_threads = no_threads;
		arguments[i].vect = array;
		arguments[i].N = numbers;
		arguments[i].start = 0;
		arguments[i].end = 0;
		arguments[i].sum = 0;
		pthread_create(&threads[i], NULL, thread_function, &arguments[i]);
	}

	for (int i = 0; i < no_threads; ++i) {
		pthread_join(threads[i], NULL);
 	}

 	int thread_sum = 0;
 	for(int i = 0; i < no_threads; i++){
 		thread_sum += arguments[i].sum;
 	}

 	printf("suma completa: %d suma threadurilor: %d", suma_completa, thread_sum);
 	printf("numarul care lipseste este: %d", (suma_completa - thread_sum));

	return 0;
}