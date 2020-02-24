#ifndef _SERVER_H_
#define _SERVER_H_

#include <vector>

#define ERROR_GAME_FULL "gameFull"
#define ERROR_COMMUNICATION "communicationError"
#define ERROR_GAME_DOES_NOT_EXIST "gameDoesNotExist"
#define ERROR_GAME_FULL "gameFull"
#define ERROR_NOT_ENOUGH_PLAYERS "notEnoughPlayers"
#define ERROR_DUPLICATE_ROLE "dupRole"

#define ACTION_GAME_LIST "gameList"
#define ACTION_CREATE_GAME "createGame"
#define ACTION_JOIN_GAME "joinGame"
#define ACTION_JOINED_GAME "joinedGame"
#define ACTION_CANT_JOIN_GAME "cantJoinGame"
#define ACTION_CHANGE_ROLE "roleChange"
#define ACTION_CANT_CHANGE_ROLE "cantChangeRole"
#define ACTION_CHANGE_MAP "changeMap"
#define ACTION_START_GAME "startGame"
#define ACTION_LOAD_LEVEL "loadLevel"
#define ACTION_CANT_START_GAME "cantStartGame"
#define ACTION_MOVE "move"
#define ACTION_ACTION "action"
#define ACTION_WIN "win"
#define ACTION_NEXT_LEVEL "nextLevel"
#define ACTION_LEAVE_GAME "leaveGame"

class Server {
    std::vector<class Game *> _games;

    public:
        Server();

        ~Server();
};

#endif