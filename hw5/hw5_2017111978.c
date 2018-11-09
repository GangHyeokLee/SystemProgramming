/*2017111978 이강혁*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXARGS 20
#define ARGLEN 100

char* makestring(char*);
void execute (char*[]);
void recognize_command(char * arglist[], char *** command, char ** file_name, int*);
void dopipe(char * arglist[], char ** command, char * file_name, int index);

void main()
{
    char *arglist[MAXARGS+1];
    int numargs = 0;
    char argbuf[ARGLEN];

    while(numargs < MAXARGS)
    {
        printf("Arg[%d]? ", numargs);
        if(fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
        {
            arglist[numargs++] = makestring(argbuf);
        }
        else
        {
            if(numargs > 0)
            {
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return;
}

void execute(char * arglist[])
{
    int pid, exitstatus, index;
    char ** command, *file_name;

    recognize_command(arglist, &command, &file_name, &index);

    pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            dopipe(arglist, command, file_name, index); //execute command
            exit(1);
        default:
            while(wait(&exitstatus)!=pid);
            printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus &0377);
    }
}

char *makestring(char *buf)
{
    char *cp;

    buf[strlen(buf)-1] = '\0';
    cp = malloc(strlen(buf)+1);
    if(cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}

void recognize_command(char * arglist[], char *** command, char ** file_name, int *index)
{
    int i;
    for(i = 0;i<MAXARGS;i++) //find pipe index
    {
        if(!(strcmp(arglist[i], "<") && strcmp(arglist[i], ">") && strcmp(arglist[i], ">>")))
        {
            break;
        }
    }

    *index = i; //save pipe index

    (*command) = (char**)calloc((*index)+1, sizeof(char*)); //make command array

    for(i = 0;i<(*index);i++) //copy command
    {
        (*command)[i] = arglist[i];
    }

    (*file_name) = arglist[(*index)+1]; //copy file name

    return;
}

void dopipe(char * arglist[], char ** command, char * file_name, int index)
{
    int fd;

    if(!(strcmp(arglist[index], "<")))
    {
        close(0); //change input
        fd = open(file_name, O_RDONLY);
        execvp(command[0], command);
        perror("execvp");
    }
    else if(!(strcmp(arglist[index], ">")))
    {
        close(1); //change output
        fd = creat(file_name, 0644); //create file
        execvp(command[0], command);
        perror("execvp");
    }
    else if(!(strcmp(arglist[index], ">>")))
    {
        close(1); //change output
        fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644); //append or create file
        execvp(command[0], command);
        perror("execvp");
    }

    return;
}