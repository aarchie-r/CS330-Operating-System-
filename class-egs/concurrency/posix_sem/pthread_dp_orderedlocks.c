// Dining Philosophers with ordered lock acquire

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

sem_t *chopsticks;
int num_threads, num_rounds;

/* Philosopher `i' tries to pick up the chopsticks.
*/
void pickup (int i)
{
	if (i > (i+1)%num_threads) {
	   	sem_wait (&chopsticks[i]);
		//sleep(1);
	   	sem_wait (&chopsticks[(i+1)%num_threads]);
        }
	else {
		sem_wait (&chopsticks[(i+1)%num_threads]);
		//sleep(1);
		sem_wait (&chopsticks[i]);
	}
}

/* Philosopher `i' puts down the chopsticks.
*/
void putdown (int i)
{
	sem_post (&chopsticks[(i+1)%num_threads]);
	sem_post (&chopsticks[i]);
}

void *solver (void *param)
{
	int i, id = *(int*)(param);
	struct timeval tv0, tv1;
        struct timezone tz0, tz1;
	
	gettimeofday(&tv0, &tz0);	
	for (i=0; i<num_rounds; i++) {
		pickup(id);
		printf("Thread %d eating!\n", id);
		sleep(1);
		putdown(id);
		printf("Thread %d thinking!\n", id);
		sleep(1);
	}
	gettimeofday(&tv1, &tz1);
        printf("Time from %d: %ld\n", id, (tv1.tv_sec-tv0.tv_sec)*1000000+(tv1.tv_usec-tv0.tv_usec));
}	

int main (int argc, char *argv[])
{
	int i, *id;
	pthread_t *tid;
	pthread_attr_t attr;
	struct timeval tv0, tv1;
	struct timezone tz0, tz1;

	if (argc != 3) {
		printf ("Need number of threads and number of rounds.\n");
		exit(1);
	}
	num_threads = atoi(argv[1]);
	num_rounds = atoi(argv[2]);
	tid = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
	id = (int*)malloc(num_threads*sizeof(int));
 	for (i=0; i<num_threads; i++) id[i] = i;

	chopsticks = (sem_t*)malloc(num_threads*sizeof(sem_t));
	for (i=0; i<num_threads; i++) {
		sem_init(&chopsticks[i], 0, 1);
	}

	pthread_attr_init(&attr);

	for (i=1; i<num_threads; i++) {
		pthread_create(&tid[i], &attr, solver, &id[i]);
   	}

	gettimeofday(&tv0, &tz0);
	for (i=0; i<num_rounds; i++) {
		pickup(0);
		printf("Thread 0 eating!\n");
		sleep(1);
		putdown(0);
		printf("Thread 0 thinking!\n");
		sleep(1);
	}
	gettimeofday(&tv1, &tz1);
	printf("Time from 0: %ld\n", (tv1.tv_sec-tv0.tv_sec)*1000000+(tv1.tv_usec-tv0.tv_usec));

	for (i=1; i<num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}