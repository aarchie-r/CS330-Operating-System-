// when value of n is small, one starts and ends before second one starts
// but for bigger n, overlapping happens for updating values at the same location
// overlapping/concurrency issues

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

int *p;
int n;

void* work (void *arg)
{
   printf("(%d) address pointed to by p: %p\n", getpid(), p);
   int i;
   for (i=0; i<n; i++) *p = *p + 1;
   return NULL;
}

int main (int argc, char *argv[])
{
   if (argc != 2) {
      fprintf(stderr, "usage: vmem2 <value>\n");
      exit(1);
   }
   n = atoi(argv[1]);
   p = (int*)malloc(sizeof(int));
   assert(p != NULL);
   printf("(%d) address pointed to by p: %p\n", getpid(), p);
   *p = 0;
   pthread_t p1, p2;
   pthread_create(&p1, NULL, work, NULL);
   pthread_create(&p2, NULL, work, NULL);
   pthread_join(p1, NULL);
   pthread_join(p2, NULL);
   
   printf("Final value : %d\n", *p);

   return 0;
}