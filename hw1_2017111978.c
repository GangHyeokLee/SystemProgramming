#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <stdlib.h>
#define BUFFERSIZE      4096
#define COPYMODE        0644    

void do_ls(char[]);

int main(int ac, char *av[])
{
	if(ac==1)
		do_ls( "." );
	else
		while(--ac){
			printf("%s:\n", *++av);
			do_ls( *av);
		}
}

void do_ls(char dirname[])
/*
 *		list files in directory called dirname
 */
{
	DIR			 *dir_ptr;			/* the directory */
	struct dirent *direntp;			/* each entry	 */


	if((dir_ptr = opendir(dirname))==NULL)
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		while((direntp = readdir(dir_ptr))!=NULL)
			printf("%s\n", direntp->d_name);
		closedir(dir_ptr);
	}

}

/*cp1.c
 *		version 1 of cp - uses read and write with tunable buffer size
 *
 *		usage: cp1 src dest
 */

void oops(char *, char *);
 
int cp1(char *, char *av[])
{
        int     in_fd, out_fd, n_chars;
        char    buf[BUFFERSIZE];
 
        if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
            oops("Error while opening ", av[1]);
       
        if ( (out_fd=creat(av[2], COPYMODE)) == -1 )
            oops("Error while creat", av[2]);
 
        while ( (n_chars = read(in_fd, buf, BUFFERSIZE)) > 0 )
            if ( write( out_fd, buf, n_chars ) != n_chars )
                    oops("Write error to ", av[2]);
        if ( n_chars == -1 )
                    oops("Read error from ", av[1]);
 
        if ( close(in_fd) == -1 || close(out_fd) == -1 )
            oops("Error closing files", "");
       
        return 0;
}
 
void oops(char *s1, char *s2)
{
    fprintf(stderr,"Error: %s ", s1);
    perror(s2);
    exit(1);
}

