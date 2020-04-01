#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


// driver code
int main()
{
    int pid;

    /* get child process */
    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    { /* child */
        char *args[] = {"./child", NULL};
        execvp(args[0], args);
    }
    else /* parent */
    {    /* pid hold id of child */
        while(1){
            printf("Parent\n");
            sleep(1);
        }
        wait(NULL);
    }
    return 0;
}