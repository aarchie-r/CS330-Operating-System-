#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/time.h>

int *p;
int n;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* work (void *arg)
{
   printf("(%d) address pointed to by p: %p\n", getpid(), p);
   int i, local=0;
   for (i=0; i<n; i++) local = local + 1; // --------------allocated on local reg not on cache unlike pointer
   pthread_mutex_lock(&mutex);
   *p = *p + local; //----------------------each thread wants shared memory variable only once and hence faster than buggy code
   pthread_mutex_unlock(&mutex);
   return NULL;
}

int main (int argc, char *argv[])
{
   struct timeval tp_start, tp_end;
   if (argc != 2) {
      fprintf(stderr, "usage: race_fixed <value>\n");
      exit(1);
   }
   n = atoi(argv[1]);
   p = (int*)malloc(sizeof(int));
   assert(p != NULL);
   printf("(%d) address pointed to by p: %p\n", getpid(), p);
   *p = 0;
   pthread_t p1, p2;
  
   gettimeofday(&tp_start, NULL);
   pthread_create(&p1, NULL, work, NULL);
   pthread_create(&p2, NULL, work, NULL);
   pthread_join(p1, NULL);
   pthread_join(p2, NULL);
   gettimeofday(&tp_end, NULL);
   
   printf("Final value : %d, Time: %ld microseconds\n", *p, tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));

   return 0;
}