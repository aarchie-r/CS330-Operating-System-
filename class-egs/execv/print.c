#include <stdio.h>
#include <stdlib.h>

int main (void)
{	
	// Overlays new text, data, stack regions on top of the old regions, sets up the region table pointers correctly
	printf("Pirates of Silicon Valley is an interesting movie!\n");
	printf("My pid: %d\n", getpid());
	printf("my parent is %d\n", getppid());
	return 0;
}
