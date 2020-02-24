#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define BUFFER_SIZE 8192

#define ERR -1
#define NO_ERROR "No Error"
#define SOCKET_ERROR "Error when creating socket"
#define BIND_ERROR "Error when binding"
#define LISTEN_ERROR "Error when listening"
#define ACCEPT_ERROR "Error when accepting"
#define RECEIVE_ERROR "Error when receiving"
#define SEND_ERROR "Error when sending"
#define MAX_CONNECTION_ERROR "Maximum connection reached"

#define syserror(x) perror(x)
#define fatalsyserror(x) syserror(x), exit(1)

#endif