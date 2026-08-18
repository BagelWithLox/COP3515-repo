/*
 * Simple Fork Example
 *
 * Note: if needed, run from shell
 */

#include <stdio.h>
#include <unistd.h>

void forkexample()
{
    int PID;
    // child process because return value zero
    PID = fork();
    if (PID == 0) {
        printf("\nHello from Child!\n");
        printf("Child PID: %d\n",PID);
    }
    // parent process because return value non-zero.
    else {
        printf("\nHello from Parent!\n");
        printf("Parent PID: %d\n",PID);
    }
}
int main()
{
    forkexample();
    return 0;
}
