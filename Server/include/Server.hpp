#ifndef _SERVER_H_
#define _SERVER_H_

#include <vector>
#include <string>
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
    void requestGamesList();
    void requestChangeRole(int roleID);
    void requestChangeMap(std::string mapName);
    void requestAction();
    void requestCreateGame();
    void requestJoinGame(int gameID);
    void requestStartGame();
    void requestMove(std::string moveDir);
    void requestNextLevel();
    void requestLeaveGame();
};

#endif