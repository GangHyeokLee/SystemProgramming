#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE      4096
#define COPYMODE        0644    

void do_ls(char*, char*);
void oops(char*, char*);
void cp1(char*,char*, char*);

void main(int ac, char *av[])
{
	if(ac < 3)
	{
		fprintf(stderr, "usage : %s source destination\n", *av);
		exit(1);
	}

	do_ls(av[1], av[2]);


}

void do_ls(char * sourcename, char * destinationname)
/*
 *		list files in directory called dirname
 */
{
	DIR			 *dir_ptr;			/* the directory */
	struct dirent *direntp;					/* each entry	 */


	if((dir_ptr = opendir(sourcename))==NULL)
		fprintf(stderr, "ls1: cannot open %s\n", sourcename);
	else
	{
		while((direntp = readdir(dir_ptr))!=NULL)
                        if(strcmp(direntp->d_name, ".") && strcmp(direntp->d_name, ".."))
			cp1(sourcename, direntp->d_name, destinationname);
		closedir(dir_ptr);
	}

}
 
void cp1(char *sourcename, char *filename, char * destinationname)
{
        int     in_fd, out_fd, n_chars;
        char    buf[BUFFERSIZE], destination[BUFFERSIZE]={0}, sourcenamef[BUFFERSIZE]={0};

        strcpy(destination, destinationname);
	strcpy(sourcenamef, sourcename);
        strcat(sourcenamef, "/");
        strcat(sourcenamef, filename);

        if ( (in_fd=open(sourcenamef, O_RDONLY)) == -1 )
            oops("Error while opening ", filename);

        strcat(destination, "/");
       strcat(destination, filename);

        if  ((out_fd=creat(destination,COPYMODE)) == -1 )
            oops("Error while creat", destination);

        while ((n_chars = read(in_fd, buf, BUFFERSIZE))>0 )
        {
            if ( write( out_fd, buf, n_chars ) != n_chars )
                    oops("Write error to ", destination);
        }
 
        if ( n_chars == -1 )
                    oops("Read error from ", filename);
 
        if ( close(in_fd) == -1 || close(out_fd) == -1 )
            oops("Error closing files", "");
       
        return;
}
 
void oops(char *s1, char *s2)
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}

