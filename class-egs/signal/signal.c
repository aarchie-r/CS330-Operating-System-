#include <signal.h>
#include <stdio.h>

int main (void)
{
   int i, mynum;

   printf("group before = %d\n", getpgrp());
   setpgrp(); // sets group id to its own pid; returns a new group id

   printf("parent pid: %d, pgrp: %d\n", getpid(), getpgrp());
	
   for (i=0; i<5; i++) {
      if (fork() == 0) {
         printf("group before = %d\n", getpgrp());

         // odd child are made to be in different group of the parent
         if (i & 1) {
            setpgrp();  
	      }

         printf("[%d] child pid: %d, child pgrp: %d\n", i, getpid(), getpgrp());
         mynum = i;
               /* The even-numbered children get killed while executing the following
                  infinite loop. Each of the odd-numbered children has its own process
                  group and remain stuck in the following infinite loop.
               */
         // while (1) {
         //    sleep(5);
         //    printf("[%d] child pid: %d, child pgrp: %d\n", mynum, getpid(), getpgrp());
         // }
      }
   }
   /* Only the parent process reaches the following statements and sends SIGINT
      to all processes in its process group (i.e., the even-numbered children
      and itself).
   */
   sleep(10);
 
   kill (0, SIGINT); // zero if passed as pid, the signal is sent to all processes in sender's process group

   printf("Killed all in group %d\n", getpgrp()); /* Never printed */

   return 0;
}