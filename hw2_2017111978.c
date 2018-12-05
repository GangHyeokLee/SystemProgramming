#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>


void main(int argc, char* argv[])
{
	char destination[2000];
	struct dirent *exist_check;
	DIR     *dir_ptr;

	if(argc<3)
	{
		fprintf(stderr,"argc < 3\n");
		exit(1);
	}

	if(!strcmp(argv[1], argv[2])) //if current name and chaneged name is same, make error
	{
		fprintf(stderr, "same file name\n");
		
		exit(1);
	}

	strcpy(destination, argv[2]);

	if(argv[2][0] == '.')
	{
		strcat(destination, "/");
		strcat(destination, argv[1]);
	}
	else
	{
		dir_ptr = opendir(".");
		while((exist_check = readdir(dir_ptr))!=NULL)
		{
			if(!strcmp(exist_check->d_name, destination))
			{
				fprintf(stderr,"already exist\n");
				exit(1);
			}
		}
		closedir(dir_ptr);
	}

	if((rename(argv[1], destination)) == -1)
	{
		fprintf(stderr, "rename fail : ");
		perror(destination);
		exit(1);
	}

	return;

}
