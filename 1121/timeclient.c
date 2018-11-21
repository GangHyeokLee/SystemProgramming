#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "socklib.h"

#define oops(msg)  {perror(msg); exit(1);}

void main(int argc, char *  argv[])
{
	int sock_id;
	char message[BUFSIZ];
	int messlen;
	
	sock_id = connect_to_server(argv[1], atoi(argv[2]));

	messlen = read(sock_id, message, BUFSIZ);

	if(messlen == -1)
		oops("read");

	if(write(1, message, messlen) != messlen)
		oops("write");

	close(sock_id);
}
