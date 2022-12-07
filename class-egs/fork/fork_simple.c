#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

int x;

int main (void)
{
	int a, b, *c;
	a = 1;
	x = 2;
	c = (int*)malloc(sizeof(int));
	printf("Good morning. I am %d. My parent is %d.\n", getpid(), getppid());

	b = fork();
	if (b < 0) {
		printf("Error in fork. Aborting...\n");
		exit(0);
	}

	printf("After fork. I am %d. My parent is %d.\n", getpid(), getppid());

	if (b == 0) { x++; a++; *c = 10; }
	else { x--; a--; *c = 100; }

    // even if the address shows same value it is virtual address

	printf("I am %d. My parent is %d. My x is %d. My a is %d. My c is %p. My *c is %d.\n", getpid(), getppid(), x, a, c, *c);

	return 0;
}


// gcc -O3 fork_simple.c -o fork_simple