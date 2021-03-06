#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <netinet/in.h>
#include <string>

#define MAX_CONNECTION_TCP 256

class TCPConnection
{
	int fd;
	int nbConnection;
	struct sockaddr_in srv;

	socklen_t len;

public:
	int new_fd[MAX_CONNECTION_TCP];
	std::string answers[MAX_CONNECTION_TCP];
	struct sockaddr_in cli[MAX_CONNECTION_TCP];

public:
	TCPConnection();
	~TCPConnection();

	void server_bind(int port);
	void server_listen();
	int server_accept();
	std::string server_receive(int index);
	int server_send(int index);
	int server_send(int index, std::string msg);
};

#endif