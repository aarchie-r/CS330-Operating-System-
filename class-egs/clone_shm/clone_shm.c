#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <sys/time.h>

#define CHILD_STACK 16384
#define SIZE 1000000000

int *a;
unsigned long long child_sum=0, parent_sum=0;

int add (void *arg)
{
	int i;
	for (i=0; i<SIZE/2; i++) child_sum += a[i];
	printf("[%d child of (%d)] Child saying: my sum is %llu\n", getpid(), getppid(),child_sum);
}

int main (void)
{
	int i;
	void *child_stack;
        struct timeval tp_start, tp_end;

        a = (int*)malloc(SIZE*sizeof(int));
	for (i=0; i<SIZE; i++) a[i] = i;
	
        child_stack = malloc(CHILD_STACK);

        gettimeofday(&tp_start, NULL);

    clone (add, child_stack+CHILD_STACK, SIGCHLD | CLONE_VM, NULL); //like fork as there is no CLONE_THREAD flag
    /*  
    clone-args ::
        function :: int f(void*)
        starting stack addr
        share what :: integer flag -> bitwise ored => lowest 8 bits i.e., 1 byte specify the signal delivered to parent when child terminates (may be to wake up if waiting)
        void* arg to funct
    */
   /*
   child_stack+CHILD_STACK :-
        as malloc grows down to up
        stack grows up to down
        child_stack is returned with starting of allocate region
        to get staring of stack we need to add the CHILD_STACK
   */

	for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
	printf("[%d child of (%d)] Parent saying: my sum is %llu\n", getpid(), getppid(), parent_sum);

	// wait for child
	wait (NULL);

        printf("Total sum: %llu\n", child_sum+parent_sum);
        gettimeofday(&tp_end, NULL);
        printf("Total time: %ld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	return 0;
}

// gcc -O3 clone_shm.c -o clone_shm