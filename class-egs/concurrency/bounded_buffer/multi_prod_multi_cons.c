//correct implementation of multiple producers, multiple consumers bounded buffer with reasonable concurrency

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 20

typedef struct {
   int x;
   int full;
   pthread_mutex_t lock;
   pthread_cond_t inserted;
   pthread_cond_t deleted;
} buffer_elem;

buffer_elem buffer[SIZE];
int tail = 0, head = 0, num_items, num_prods, num_cons;
pthread_mutex_t lock_delete = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_insert = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_print = PTHREAD_MUTEX_INITIALIZER;

int produce (int index, int tid)
{
   return index+(num_items*tid);
}

void *producer (void *param)
{
   int i, tid = *(int*)param, index;

   for (i=0; i<num_items; i++) {

      pthread_mutex_lock(&lock_insert);
      index = tail;
      tail = (tail + 1) % SIZE;
      pthread_mutex_unlock(&lock_insert);

      pthread_mutex_lock(&buffer[index].lock);
      while (buffer[index].full) pthread_cond_wait(&buffer[index].deleted, &buffer[index].lock); //-----------------------
      buffer[index].x = produce(i, tid);
      buffer[index].full = 1;
      pthread_cond_signal(&buffer[index].inserted);
      pthread_mutex_unlock(&buffer[index].lock);
   }
}

void *consumer (void *param)
{
   int i, v, index;

   for (i=0; i<(num_items*num_prods)/num_cons; i++) {
      pthread_mutex_lock(&lock_delete);
      index = head;
      head = (head + 1) % SIZE;
      pthread_mutex_unlock(&lock_delete);

      pthread_mutex_lock(&buffer[index].lock);
      while (!buffer[index].full) pthread_cond_wait(&buffer[index].inserted, &buffer[index].lock);
      v = buffer[index].x;
      buffer[index].full = 0;
      pthread_cond_signal(&buffer[index].deleted);
      pthread_mutex_unlock(&buffer[index].lock);
      
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
	for (i=0; i<SIZE; i++) {
        	buffer[i].x = -1;
		buffer[i].full = 0;
		pthread_mutex_init(&buffer[i].lock, NULL);
		pthread_cond_init(&buffer[i].inserted, NULL);
		pthread_cond_init(&buffer[i].deleted, NULL);
	}
	for (i=0; i<num_prods; i++) pthread_create(&threads[i], &attr, producer, &tid[i]);
	for (i=0; i<num_cons-1; i++) pthread_create(&threads[i+num_prods], &attr, consumer, NULL);

	consumer(NULL);

	for (i=0; i<num_prods+num_cons-1; i++) pthread_join(threads[i], NULL);
	printf("\n");

	return 0;
}



//The bounded buffer example would initialize the semaphore to k if the size of the buffer is k