#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void main(int argc, char * argv[])
{
	int sock, fd;
	FILE * fpin;
	char request[BUFSIZ];

	if(argc == 1)
	{
		fprintf(stderr, "usage:ws portnum\n");
		exit(1);
	}
	sock = make_server_socket(atoi(av[1]));

	if(sockI == -1)
	       	exit(2);

	//main loop
	while(1)
	{
		fd = accept(sock, NULL, NULL);
		fpin = fdopen(fd, "r");

		fgets(request, BUFSIZ, fpin);
		printf("got a call: request = %s", request);
		read_til_crnl(fpin);

		process_rq(request, fd);

		fclose(fpin);
	}
}

read_til_crnl(FILE* fp)
{
	char buf[BUFSIZ];

	while(fgets(buf, BUFSIZ, fp)!=NULL && strcmp(buf, "\r\n")!=0);
}

process_rq(char * rq, int fd)
{
	char cmd[BUFSIZ], arg[BUFSIZ];

	if(fork()!=0)
		return;

	strcpy(arg,"./");

	if(sscanf(rq, "%s%s", cmd, arg+2)!=2)
		return;

	if(strcmp(cmd, "GET")!=0)
		cannot_do(fd);
	else if(not_exist(arg)) do_404(arg, fd);
	else if(isadir(arg)) 	do_ls(arg, fd);
	else if(end_in_cgi(arg))do_exec(arg, fd);
	else  		 	do_cat(arg, fd);
}

void do_ls(char * dir, int fd)
{
	FILE * fp;
}
