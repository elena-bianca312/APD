#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 4
#define TIME_LEN 9

sem_t semaphore;

char *get_time(char *str) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(str, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

	return str;
}

void *f(void *arg) {
	long id = *(long*) arg;
	char time_str[TIME_LEN];
	
	printf("[%s] T%ld vrea sa intre in zona critica...\n", get_time(time_str), id);
	sem_wait(&semaphore);

	// zona critica
	printf("[%s] T%ld a intrat in zona critica >\n", get_time(time_str), id);
	sleep(5);

	sem_post(&semaphore);
	printf("[%s] T%ld a iesit din zona critica <\n", get_time(time_str), id);

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
	int r;
	long id;
	void *status;
	long ids[NUM_THREADS];

	sem_init(&semaphore, 0, NUM_THREADS - 1);

	for (id = 0; id < NUM_THREADS; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

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

	sem_destroy(&semaphore);

	return 0;
}
