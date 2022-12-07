#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include <errno.h> 
#include <signal.h>
#include <assert.h>

void sighandler1(int);
void sighandler2(int);
void (*handler)(int);   // Function pointer

void sighandler1(int signo)
{
//    handler = &sighandler2;
   printf("I am SIGINT in handler 1\n");
//    exit(0);
}

void sighandler2(int signo)
{
//    handler = &sighandler1;
   printf("I am SIGINT in handler 2\n");
}

int main()
{
   handler = &sighandler1;   // Assign function pointer
   
   for(int i=1;i<=19;i++){
       signal(i, handler);
   }

    while(1){
        printf("[%d]\n",getpid());
    }
    exit(0);
}