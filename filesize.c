#include <stdio.h>
#include <sys/stat.h>

int main(int ac, char *av[])
{
	struct stat infobuf;
	if(stat(av[1], &infobuf)==-1)
		perror(av[1]);
	else
		printf("The size of %s is %d\n", av[1],  infobuf.st_size);
}

