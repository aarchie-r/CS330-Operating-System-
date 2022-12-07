#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

int main (void)
{
        char *buffer;
	int i;

	buffer = (char*)mmap(NULL, sizeof(char)*1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (buffer == MAP_FAILED) {
		printf("mmap failed!\nAborting...\n");
		exit(1);
	}

        for (i=0; i<100; i++) {
           printf("%c", buffer[i]);
        }
        printf("\n");

        for (i=0; i<10; i++) {
           buffer[i] = 'a'+i;
        }

	for (i=0; i<100; i++) {
           printf("%c", buffer[i]);
        }
        printf("\n");

	munmap(buffer, sizeof(char)*1000);
	return 0;
}