#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 20

int buffer[SIZE], tail = 0, head = 0, num_items, num_in_buffer = 0;
// head for consumer, tail for producer
// in each empty space producer produces a value and consumer uses it making it again empty
// buffer is circular


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t inserted = PTHREAD_COND_INITIALIZER;
pthread_cond_t deleted = PTHREAD_COND_INITIALIZER;

int produce (int index)
{
   return index;
}

void *producer (void *param)
{
   int i;

   for (i=0; i<num_items; i++) {
      pthread_mutex_lock(&lock);
      while (num_in_buffer == SIZE) pthread_cond_wait(&deleted, &lock);
      buffer[tail] = produce(i);
      num_in_buffer++;
      pthread_cond_signal(&inserted);
      pthread_mutex_unlock(&lock);
      tail = (tail + 1) % SIZE;
   }
}

void *consumer (void *param)
{
   int i, v;

   for (i=0; i<num_items; i++) {
      pthread_mutex_lock(&lock);
      while (num_in_buffer == 0) pthread_cond_wait(&inserted, &lock);
      v = buffer[head];
      num_in_buffer--;
      pthread_cond_signal(&deleted);
      pthread_mutex_unlock(&lock);
      head = (head + 1) % SIZE;
      printf("%d ", v);
   }
   printf("\n");
}

int main (int argc, char *argv[])
{
	int i, j;
	pthread_t *threads;
	pthread_attr_t attr;

	if (argc != 2) {
		printf ("Need number of items to be produced.\n");
		exit(1);
	}
	num_items = atoi(argv[1]);
	threads = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_attr_init(&attr);
	pthread_create(threads, &attr, producer, NULL);

	consumer(NULL);

	return 0;
}