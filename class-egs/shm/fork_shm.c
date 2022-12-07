#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> //shared memory
#include <sys/time.h>

#define SUM_KEY 19

#define SIZE 1000000000

int main (void)
{
	int *a, i, shm_id;
	char *shm;
	unsigned long long child_sum=0, parent_sum=0;
        struct timeval tp_start, tp_end;

    a = (int*)malloc(SIZE*sizeof(int));
	for (i=0; i<SIZE; i++) a[i] = i;

	shm_id = shmget(SUM_KEY, 4096, 0777 | IPC_CREAT); // syscall to get shared memory descriptor

    gettimeofday(&tp_start, NULL);
 
	if (fork() == 0) {
		// This is child

		for (i=0; i<SIZE/2; i++) child_sum += a[i];
		printf("Child saying: my sum is %llu\n", child_sum);

		shm = (char*)shmat(shm_id, 0, 0); //attaches shared memory to process return start pointer (_,starting addr,flag)
                                            // flag 0 = all option left entirely to kernel
                                            // strating addr may be ignored by kernel

		*(unsigned long long*)shm = child_sum; // not system call

    }
    else {
		for (i=SIZE/2; i<SIZE; i++) parent_sum += a[i];
		printf("Parent saying: my sum is %llu\n", parent_sum);

		shm = (char*)shmat(shm_id, 0, 0);

		wait(NULL);

                printf("Total sum: %llu\n", *((unsigned long long*)shm)+parent_sum);
                gettimeofday(&tp_end, NULL);

		shmctl(shm_id, IPC_RMID, NULL); // querying & updating the status of shared mem region 
        // can be used anytime
        // region table is updated to indicate that when attach count reaches to 0, region should be deallocatted
        // after IPC_RMID no process can attach to the region i.e., shared mem table doesn't exit anymore or *removes descriptor*

        printf("Total time: %ld microseconds\n", tp_end.tv_sec*1000000+tp_end.tv_usec - (tp_start.tv_sec*1000000+tp_start.tv_usec));
	}

	shmdt(shm); // arg = return value of shmat
    // if pointer is passed it has freedom to execute anytime (even after shmdt)
    // else for descriptor should have order before shmdt
    //decrements the attach count by one (also done by exit()) & updates time of last detach

	return 0;
}