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

/* Client actions */
#define ACTION_CREATE_GAME "createGame"
#define ACTION_JOIN_GAME "joinGame"
#define ACTION_START_GAME "startGame"
#define ACTION_MOVE "move"
#define ACTION_NEXT_LEVEL "nextLevel"
#define ACTION_LEAVE_GAME "leaveGame"
#define ACTION_CHANGE_MAP "changeMap"
#define ACTION_RETURN_LOBBY "returnToLobby"

/* Server actions */
#define ACTION_CREATED_GAME "createdGame"
#define ACTION_JOINED_GAME "joinedGame"
#define ACTION_CANT_JOIN_GAME "cantJoinGame"
#define ACTION_CANT_CHANGE_ROLE "cantChangeRole"
#define ACTION_LOAD_LEVEL "loadLevel"
#define ACTION_CANT_START_GAME "cantStartGame"
#define ACTION_WIN "win"
#define ACTION_CHANGED_MAP "changedMap"
#define ACTION_RETURNED_LOBBY "returnToLobby"


/* Client & Server actions */
#define ACTION_GAMES_LIST "gameList"
#define ACTION_CHANGE_ROLE "roleChange"
#define ACTION_ACTION "action"
#define ACTION_SEND_PING "sendPing"

#define syserror(x) perror(x)
#define fatalsyserror(x) syserror(x), exit(1)

#endif