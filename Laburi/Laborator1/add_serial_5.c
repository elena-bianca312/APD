#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    schelet pentru exercitiul 5
*/

int* arr;
int array_size;

// Structura ce contine capetele vectorului prelucrat intr-un thread
struct pos{
    int start;
    int end;
};

// Functia pentru un thread
void *f(void *arg) {
  	struct pos s = *(struct pos*)arg;
    for (int i = s.start; i < s.end; i++) {
        arr[i] = arr[i] + 100;
    }
	pthread_exit(NULL);
}

// Functie de min
int min(int a, int b){
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    //
    long P = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[P];
  	int r;
  	long id;
  	void *status;
	long ids[P];

    struct pos p[P];
    for (id = 0; id < P; id++) {
        p[id].start = (id * (double)array_size) / P;
        p[id].end = min((id + 1) * (double)array_size / P, array_size);
        ids[id] = id;

		r = pthread_create(&threads[id], NULL, f, &p[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
    }

    for (id = 0; id < P; id++) {
        r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
    }
    //

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
    
  	pthread_exit(NULL);
}
