//correct implementation of multiple producers, multiple consumers bounded buffer with no concurrency

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 20

int buffer[SIZE], tail = 0, head = 0, num_items, num_prods, num_cons, num_in_buffer = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_print = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t inserted = PTHREAD_COND_INITIALIZER;
pthread_cond_t deleted = PTHREAD_COND_INITIALIZER;

int produce (int index, int tid)
{
   return index+(num_items*tid);
}

void *producer (void *param)
{
   int i, tid = *(int*)param;

   for (i=0; i<num_items; i++) {
      pthread_mutex_lock(&lock);
      while (num_in_buffer == SIZE) pthread_cond_wait(&deleted, &lock);
      buffer[tail] = produce(i, tid);
      num_in_buffer++;
      tail = (tail + 1) % SIZE;
      pthread_cond_signal(&inserted);
      pthread_mutex_unlock(&lock);
   }
}

void *consumer (void *param)
{
   int i, v;

   for (i=0; i<(num_items*num_prods)/num_cons; i++) {

      pthread_mutex_lock(&lock);
      while (num_in_buffer == 0) pthread_cond_wait(&inserted, &lock);
      v = buffer[head];
      num_in_buffer--;
      head = (head + 1) % SIZE;
      pthread_cond_signal(&deleted);
      pthread_mutex_unlock(&lock);

      pthread_mutex_lock(&lock_print);
      printf("%d ", v); fflush(stdout);
      pthread_mutex_unlock(&lock_print);
   }
}

int main (int argc, char *argv[])
{
	int i, *tid;
	pthread_t *threads;
	pthread_attr_t attr;

	if (argc != 4) {
		printf ("Need number of items to be produced by each producer, number of producers, number of consumers.\n");
		exit(1);
	}
	num_items = atoi(argv[1]);
	num_prods = atoi(argv[2]);
	num_cons = atoi(argv[3]);
	threads = (pthread_t*)malloc((num_prods+num_cons-1)*sizeof(pthread_t));
	pthread_attr_init(&attr);
 	tid = (int*)malloc((num_prods+num_cons-1)*sizeof(int));
	for (i=0; i<num_prods+num_cons-1; i++) tid[i] = i;
	for (i=0; i<SIZE; i++) buffer[i] = -1;
	for (i=0; i<num_prods; i++) pthread_create(&threads[i], &attr, producer, &tid[i]);
	for (i=0; i<num_cons-1; i++) pthread_create(&threads[i+num_prods], &attr, consumer, NULL);

	consumer(NULL);

	for (i=0; i<num_prods+num_cons-1; i++) pthread_join(threads[i], NULL);
	printf("\n");

	return 0;
}