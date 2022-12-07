#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	printf("The game is never over, John. But there may be some new players now.\n");
	printf("Pid before execv: %d\n", getpid());
	printf("my parent is %d\n", getppid());
	// parent + its pid is same as the context is just updated not created

	// int execv(const char *pathname, char *const argv[]);
	execv("print",NULL); // same context, so same pid 

	printf("Returned from execv call.\n"); // never returned and thus not printed
	return 0;
}


// gcc -O3 execv_test.c -o execv_test
// gcc -O3 print.c -o print