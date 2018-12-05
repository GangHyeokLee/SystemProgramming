#include <unistd.h>
#include <stdio.h>

void main()
{
	pid_t pid = getpid();

	printf("After execvp(): %d\n", pid);

	return;
}
