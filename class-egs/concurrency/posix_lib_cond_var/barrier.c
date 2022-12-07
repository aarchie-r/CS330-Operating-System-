#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int num_threads, num_iters, count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock_print = PTHREAD_MUTEX_INITIALIZER;

void barrier (int P)
{
   pthread_mutex_lock(&lock);

   count++;

   if (count == P) {
      count = 0;
      pthread_cond_broadcast(&cv1); // broadcast to wake all threads waiting
   }
   else pthread_cond_wait(&cv1, &lock); //----- makes sure lock is released before going to sleep
                                        // ----- also reaquires lock at the time of wake up
                                        /* 
                                        void wait(cond var *v, lock *l){
                                            unlock(l);
                                            enque process and goes to sleep
                                            after waking & scheduled -> lock(l);
                                        }
                                        */
   pthread_mutex_unlock(&lock);
}

void *f (void *param)
{
	int i, id = *(int*)(param);
	for (i=0; i<num_iters; i++) {
           pthread_mutex_lock(&lock_print);
           printf("%d: entry to barrier#%d\n", id, i);
           pthread_mutex_unlock(&lock_print);

           barrier (num_threads);

	   pthread_mutex_lock(&lock_print);
           printf("%d: exit from barrier#%d\n", id, i);
           pthread_mutex_unlock(&lock_print);
        }
}	

int main (int argc, char *argv[])
{
	int i, j, *tid;
	pthread_t *threads;
	pthread_attr_t attr;

	if (argc != 3) {
		printf ("Need number of threads and number of iterations.\n");
		exit(1);
	}
	num_threads = atoi(argv[1]);
        num_iters = atoi(argv[2]);

	threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
        tid = (int*)malloc(num_threads*sizeof(int));
        for (i=0; i<num_threads; i++) tid[i] = i;

	pthread_attr_init(&attr);

	for (i=1; i<num_threads; i++) {
                /* pthread_create arguments: thread pointer,
                                             attribute pointer,
                                             function pointer,
                                             argument pointer to the function
                */
		pthread_create(&threads[i], &attr, f, &tid[i]);
   	}
        f (&tid[0]);
	
	for (i=1; i<num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}