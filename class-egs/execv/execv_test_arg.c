#include <stdio.h>
#include <stdlib.h>

int main (void)
{
        char *argv[5];
        argv[0] = "./caesar-cipher"; // first agrument must be the file-path of executable
        argv[1] = "ZHOFRPH";
        argv[2] = "WR";
        argv[3] = "FV663";
 	argv[4] = NULL; // last argument must be "null" for termination

	int s = execv("caesar-cipher",argv);

	printf("Returned from execv call with error %d\n", s); // never printed
	return 0;
}

//  gcc -O3 execv_test_arg.c -o execv_test_arg
// gcc -O3 caesar-cipher.c -o caesar-cipher