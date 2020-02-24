#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <netinet/in.h>

#define MAX_CONNECTION_TCP 256

class TCPConnection
{
	int fd;
	int new_fd[MAX_CONNECTION_TCP];
	int nbConnection;
	struct sockaddr_in srv;
	struct sockaddr_in cli[MAX_CONNECTION_TCP];
	socklen_t len;

public:
	TCPConnection();
	~TCPConnection();

	void server_bind(int);
	void server_listen();
	int server_accept();
	ssize_t server_receive(int, char *, size_t);
	int server_send(int, char *);
};

#endif