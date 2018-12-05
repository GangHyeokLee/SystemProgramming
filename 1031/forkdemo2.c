#include <stdio.h>
#include <unistd.h>

void main()
{
	printf("my pid is %d\n", getpid());
	fork();
	printf("1\n");
	fork();
	printf("2\n");
	fork();
	printf("3\n");
	printf("my pid is %d\n", getpid());
}
