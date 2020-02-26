#ifndef _SERVER_H_
#define _SERVER_H_

#include <vector>
#include "TCPConnection.hpp"

#define ERROR_GAME_FULL "gameFull"
#define ERROR_COMMUNICATION "communicationError"
#define ERROR_GAME_DOES_NOT_EXIST "gameDoesNotExist"
#define ERROR_GAME_FULL "gameFull"
#define ERROR_NOT_ENOUGH_PLAYERS "notEnoughPlayers"
#define ERROR_DUPLICATE_ROLE "dupRole"

#define TCP_PORT 1789

class Server
{
    std::vector<class Game *> _games;
    TCPConnection TCPConn;

private:
    void runPlayer(int fd);

public:
    Server();
    ~Server();

    void run();

    /* Called after a request from the client */
    /* TO DO: Methods parameters */
    void requestGamesList();
    void requestChangeRole();
    void requestChangeMap();
    void requestAction();
    void requestCreateGame();
    void requestJoinGame();
    void requestStartGame();
    void requestMove();
    void requestNextLevel();
    void requestLeaveGame();
};

#endif