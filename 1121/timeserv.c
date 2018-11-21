#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include "socklib.h"

#define PORTNUM 13785
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

void main(int argc, char * argv[])
{
	int sock_id, sock_fd;
	char *ctime();
	time_t thetime;

	sock_id = make_server_socket(PORTNUM);

	if(sock_id == -1)
		exit(1);

	while(1)
	{
		sock_fd = accept(sock_id, NULL, NULL);
		printf("Wow! got a call!\n");
		if(sock_fd == -1)
			oops("accept");

		process_request(sock_fd);
		close(sock_fd);

	}
}


