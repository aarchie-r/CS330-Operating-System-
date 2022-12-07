#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/syscall.h>
int x=6, y=-1, z=-1;

int f(void *arg){
    if (z == getpid()) {
        x++;
        printf("%d\n", x); fflush(stdout); // Created thread prints 4
    }
    else {
        x += 3; // x=9
        if (y == syscall(SYS_gettid)) { // tid of process == pid of process != pid of child
        printf("%d\n", x); fflush(stdout);
        }
        else {
            x--; // x=8
            printf("%d\n", x); fflush(stdout); // Parent process prints 8
        }
    }
    return x;
}

int main(void){
    void *child_stack;
    y = fork(); // y= pid of child
    if (y==0) {
        z = getpid();
        x=x/2; // x=3
        child_stack = malloc(16384);
        clone(f, child_stack+16384, CLONE_VM | CLONE_THREAD | CLONE_SIGHAND, NULL);
        while(wait(NULL)==-1); // no child, the one created was sibling
        printf("%d\n", x); fflush(stdout); // never printed
    }
    else {
        wait(NULL); // terminated by the cloned thread created
        x = f(NULL);  // x=8
        printf("%d\n", x); fflush(stdout); // Parent process prints 8
    }
    x -= 4; // x=4
    printf("%d\n", x); fflush(stdout); // Parent process prints 4
    return 0;
}