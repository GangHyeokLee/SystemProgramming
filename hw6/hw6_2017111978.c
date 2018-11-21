#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define oops(m,x) {perror(m); exit(x);}

void main(int argc, char *argv[])
{
    int thepipe[2][2], newfd, pid, parentpid = getpid(); //pipe[1] : argv[1]->argv[2], pipe[2] : argv[2]->argv[3]

    if(argc != 4)
    {
        fprintf(stderr, "usage: pipe cmd1 cmd2 cmd3\n");
        exit(1);
    }
    if(pipe(thepipe[0]) == -1)
        oops("Cannot get a first pipe", 1);

    if(pipe(thepipe[1]) == -1)
        oops("Cannot get a second pipe", 1);

    printf("%d %d %d %d\n", thepipe[0][0], thepipe[0][1], thepipe[1][0], thepipe[1][1]);

    if(getpid() == parentpid) //make two child
    {
        if((pid = fork()) == -1)
        {
            oops("Cannot fork1", 2);
        }
        else
        {
            printf("fork1 succeed\n");
        }
    }

    if(getpid() == parentpid)
    {
        if((pid = fork()) == -1)
        {
            oops("Cannot fork2", 3);
        }
        else
        {
            printf("fork2 succeed\n");
        }
    }

    if(getpid() == parentpid) // parent will execute argv[3]
    {
        printf("%d\n", getpid());
        printf("parent start\n");
        close(thepipe[1][1]); //close writing end of second pipe

        //close irrelevant pipe
        close(thepipe[0][0]);
        close(thepipe[0][1]);

        if(dup2(thepipe[1][0], 0) == -1) //duplicate reading end of second pipe to stdin
        oops("Could not redirect stdin", 4);

        close(thepipe[1][0]); //close free pipe
        execlp(argv[3], argv[3], NULL);
        oops(argv[3],5);
    }
    else if(getpid() == (parentpid+1)) //first child will execute argv[2]
    {
        printf("%d\n", getpid());
        printf("child1 start\n");
        close(thepipe[1][0]); //close reading end of second pipe
        close(thepipe[0][1]); //close writing end of first pipe

        if(dup2(thepipe[1][1], 1) == -1) //duplicate writing end of second pipe to stdout
            oops("Could not redirect stdout", 6);

        if(dup2(thepipe[0][0], 0) == -1) //duplicate reading end of first pipe to stdin
            oops("Could not redirect stdout", 7);


        close(thepipe[1][1]);
        close(thepipe[0][0]);


        execlp(argv[2], argv[1], NULL);
        oops(argv[2], 8);
    }
    else if(getpid() == (parentpid+2)) //second child will execute argv[1]
    {
        printf("%d\n", getpid());
        printf("child2 start\n");
        close(thepipe[0][0]); //close reading end of first pipe

        //close irrelevant pipe
        close(thepipe[1][0]);
        close(thepipe[1][1]);

        if(dup2(thepipe[0][1], 1) == -1) //duplicate writing end of first pipe to stdout
        oops("Could not redirect stdin", 9);

        close(thepipe[0][1]); //close free pipe

        execlp(argv[3], argv[3], NULL);
        oops(argv[3],10);
    }
    return;
}