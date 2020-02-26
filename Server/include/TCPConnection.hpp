#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <netinet/in.h>

#define MAX_CONNECTION_TCP 256

class TCPConnection
{
	int fd;
	int nbConnection;
	struct sockaddr_in srv;

	socklen_t len;

public:
	int new_fd[MAX_CONNECTION_TCP];
	struct sockaddr_in cli[MAX_CONNECTION_TCP];

public:
	TCPConnection();
	~TCPConnection();

	void server_bind(int port);
	void server_listen();
	int server_accept();
	ssize_t server_receive(int fd, char *buf, size_t size);
	int server_send(int fd, char *msg);
};

#endif