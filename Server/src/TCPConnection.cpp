#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "Globals.hpp"
#include "TCPConnection.hpp"

TCPConnection::TCPConnection()
{
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == ERR)
  {
    fatalsyserror(SOCKET_ERROR);
  }
  nbConnection = 0;
  //memset(srv, 0, sizeof(struct sockaddr_in));
}

TCPConnection::~TCPConnection()
{
  for (int i = 0; i < nbConnection; ++i)
  {
    close(new_fd[i]);
  }
}

/**
Bind the tcp server to the port
*/
void TCPConnection::server_bind(int port)
{
  srv.sin_family = AF_INET;
  srv.sin_port = htons(port);
  srv.sin_addr.s_addr = INADDR_ANY;
  len = sizeof(struct sockaddr_in);
  if (bind(fd, (struct sockaddr *)&srv, len) == ERR)
  {
    close(fd);
    fatalsyserror(BIND_ERROR);
  }
}

/**
Make server listening to incomming connection
*/
void TCPConnection::server_listen()
{
  if (listen(fd, SOMAXCONN) == ERR)
  {
    close(fd);
    fatalsyserror(LISTEN_ERROR);
  }
}

/**
accept 1 connection to the server
*/
int TCPConnection::server_accept()
{
  socklen_t len = sizeof(struct sockaddr_in);
  if (nbConnection == MAX_CONNECTION_TCP)
  {
    syserror(MAX_CONNECTION_ERROR);
    return ERR;
  }
  if ((new_fd[nbConnection] =
           accept(fd, (struct sockaddr *)&cli[nbConnection],
                  &len)) == ERR)
  {
    close(fd);
    fatalsyserror(ACCEPT_ERROR);
  }
  return nbConnection++;
}

/**
read incoming message from the tcp server
*/
ssize_t TCPConnection::server_receive(int fd, char *buf, size_t size)
{
  ssize_t len;
  if ((len = recv(fd, buf, size, 0)) == ERR)
  {
    close(fd);
    syserror(RECEIVE_ERROR);
  }
  return len;
}

/**
Send message to the tcp client
*/
int TCPConnection::server_send(int fd, char *msg)
{
  if (send(fd, msg, strlen(msg), MSG_NOSIGNAL) == ERR)
  {
    close(fd);
    syserror(SEND_ERROR);
    return ERR;
  }
  return 0;
}
