// socklib.c
// This file contains functions used lots when writing internet
// client/server programs. The two functions here are:
// int make_server_socket(portnum) 
//	returns a server socket
// 	or -1 if error
// int make_server_socket_q(portnum, backlog)
// int connect_to_server(char* hostname, int portnum)
//	returns a connected socket
//	or -1 if error 

#include "socklib.h"

#define HOSTLEN 256
#define BACKLOG 1	// only one request at a time

int make_server_socket(int portnum){
	return make_server_socket_q(portnum, BACKLOG);
}

// create a socket with queue_size=1
int make_server_socket_q(int portnum, int backlog){
	struct sockaddr_in	saddr;	// build our address here
	struct hostent*		hp;	// this is part of our
	char   hostname[HOSTLEN];	// address
	int    sock_id;			// the socket

	sock_id = socket(PF_INET, SOCK_STREAM, 0); // get a socket 
						   // of the PF_INET type
	if(sock_id == -1)	return -1;

	// build address and bind it to socket
	bzero((void*)&saddr, sizeof(saddr));	// clear out struct
	gethostname(hostname, HOSTLEN);		// get the hostname
	hp = gethostbyname(hostname);		// get info about host
						// fill in host part
	bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(portnum);	// fill in socket port
	saddr.sin_family = AF_INET;		// fill in addr family

	// now bind the addr to the socket
	if(bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
		return -1;

	// arrange for incoming calls
	if(listen(sock_id, backlog) != 0) return -1;
	return sock_id;
}

// connect to the web server
int connect_to_server(char* host, int portnum){
	int sock;
	struct sockaddr_in      servadd;	// the number to call
	struct hostent*         hp;     	// used to get number
	
	// Step 1: Get a socket
	sock = socket(AF_INET, SOCK_STREAM, 0); // get a line
	if(sock == -1)	return -1;

	// Step 2: connect to server
	bzero(&servadd, sizeof(servadd));	// zero the address
	hp = gethostbyname(host);		// lookup host's IP #
	if(hp == NULL) return -1;		
	bcopy(hp->h_addr, (struct sockaddr*)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(portnum);	// fill in port number
	servadd.sin_family = AF_INET;		// fill in socket type
	if(connect(sock, (struct sockaddr*)&servadd, sizeof(servadd)) != 0)
		return -1;
	return sock;
}
