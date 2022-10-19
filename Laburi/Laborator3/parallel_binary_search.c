#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define INSIDE -1 // daca numarul cautat este in interiorul intervalului
#define OUTSIDE -2 // daca numarul cautat este in afara intervalului

// variablia globala pentru pozitia din vector pe care se afla nr cautat
int nr = -1;

struct my_arg {
	int id;
	int N;
	int P;
	int number;
	int *left;
	int *right;
	int *keep_running;
	int *v;
	int *found;
};

pthread_barrier_t barrier;

/*
void binary_search() {
	while (keep_running) {
		int mid = left + (right - left) / 2;
		if (left > right) {
			printf("Number not found\n");
			break;
		}

		if (v[mid] == number) {
			keep_running = 0;
			printf("Number found at position %d\n", mid);
		} else if (v[mid] > number) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
}
*/

int min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

void *f(void *arg) {
	struct my_arg *data = (struct my_arg *) arg;

	// Daca numarul cautat nu este in vector
	if (data->number < data->v[0] || data->number > data->v[data->N - 1]) {
		return NULL;
	}

	while (*data->keep_running) {
		int size = *data->right - *data->left + 1;

		// Offset + intervalul thread-ului actual
		int start = *data->left + data->id * (double) size / data->P;
		int end = *data->left + min((data->id + 1) * (double) size / data->P, size - 1);

		// Vedem daca numarul cautat este intr-unul dintre capetele intervalului
		if (data->v[start] == data->number) {
			nr = start;
		}

		if (data->v[end] == data->number) {
			nr = end;
		}

		// Asteptam celelalte thread-uri (ca sa nu scrie gresit left si right)
		pthread_barrier_wait(&barrier);

		// Verificam daca numarul cautat se afla in interiorul intervalului
		if (data->v[start] < data->number && data->v[end] > data->number) {
			// Actualizam left si right (ignoram capetele -> deja verificate)
			*data->left = start + 1;
			*data->right = end - 1;
		}

		// Daca am gasit numarul sau daca intervalul are dimesiunea 1
		if (nr != -1 || start == end) {
			// Ne oprim din cautat
			*data->keep_running = 0;
		}

		// Asteptam celelalte thread-uri (inaninte de urmatoarea iteratie)
		pthread_barrier_wait(&barrier);
	}

	return NULL;
}

void display_vector(int *v, int size) {
	int i;

	for (i = 0; i < size; i++) {
		printf("%d ", v[i]);
	}

	printf("\n");
}


int main(int argc, char *argv[]) {
	int r, N, P, number, keep_running, left, right;
	int *v;
	int *found;
	void *status;
	pthread_t *threads;
	struct my_arg *arguments;

	if (argc < 4) {
		printf("Usage:\n\t./ex N P number\n");
		return 1;
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
	number = atoi(argv[3]);

	keep_running = 1;
	left = 0;
	right = N - 1;

	v = (int*) malloc(N * sizeof(int));
	threads = (pthread_t*) malloc(P * sizeof(pthread_t));
	arguments = (struct my_arg*) malloc(P * sizeof(struct my_arg));
	found = (int*) malloc(P * sizeof(int));

	for (int i = 0; i < N; i++) {
		v[i] = i;
	}

	display_vector(v, N);

	r = pthread_barrier_init(&barrier, NULL, P);

	if (r) {
		printf("Error\n");
		exit(-1);
	}

	for (int i = 0; i < P; i++) {
		arguments[i].id = i;
		arguments[i].N = N;
		arguments[i].P = P;
		arguments[i].number = number;
		arguments[i].left = &left;
		arguments[i].right = &right;
		arguments[i].keep_running = &keep_running;
		arguments[i].v = v;
		arguments[i].found = found;

		r = pthread_create(&threads[i], NULL, f, &arguments[i]);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (int i = 0; i < P; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	if (nr != -1) {
		printf("%d\n", nr);
	}

	free(v);
	free(threads);
	free(arguments);

	pthread_barrier_destroy(&barrier);

	return 0;
}
