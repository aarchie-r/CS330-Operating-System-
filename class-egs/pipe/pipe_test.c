#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int pipefd1[2], pipefd2[2], x;
	char y, z, w;

	if (argc != 2) {
		printf("Syntax: pipetest char. Aborting...\n");
		exit(0);
	}

	if (pipe(pipefd1) < 0) {
		printf("Error: cannot create pipe. Aborting...\n");
		exit(0);
	}

	if (pipe(pipefd2) < 0) {
                printf("Error: cannot create pipe. Aborting...\n");
                exit(0);
        }

	x = fork(); //----------------------------------------------------------------------------

	if (x < 0) {
		printf("Error: cannot fork. Aborting...\n");
		exit(0);
	}
	else if (x > 0) {
		y = argv[1][0];
		z = y+1;
		if (write(pipefd1[1], &y, 1) < 0) {
			printf("Error: cannot write. Aborting...\n");
			exit(0);
		}
		if (read(pipefd2[0], &z, 1) < 0) {
			printf("Error: cannot read. Aborting...\n");
			exit(0);
		}
		printf("Parent[%d]: received %c\n", getpid(), z);
		// Close parent's copy of the file descriptors
		close(pipefd1[0]);
        	close(pipefd1[1]);
        	close(pipefd2[0]);
        	close(pipefd2[1]);
	}
	else {
		if (read(pipefd1[0], &w, 1) < 0) {
			printf("Error: cannot read. Aborting...\n");
                        exit(0);
                }
		if (write(pipefd2[1], &w, 1) < 0) {
			printf("Error: cannot write. Aborting...\n");
                        exit(0);
                }
		// Close child's copy of the file descriptors
                close(pipefd1[0]);
                close(pipefd1[1]);
                close(pipefd2[0]);
                close(pipefd2[1]);
	}
	return 0;
}