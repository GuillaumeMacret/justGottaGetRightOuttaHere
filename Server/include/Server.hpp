#ifndef _SERVER_H_
#define _SERVER_H_

#include <vector>
#include <string>
#include <map>
#include "TCPConnection.hpp"

#define ERROR_GAME_FULL "gameFull"
#define ERROR_COMMUNICATION "communicationError"
#define ERROR_GAME_DOES_NOT_EXIST "gameDoesNotExist"
#define ERROR_GAME_FULL "gameFull"
#define ERROR_NOT_ENOUGH_PLAYERS "notEnoughPlayers"
#define ERROR_DUPLICATE_ROLE "dupRole"

#define TCP_PORT 1789

class Game;

class Server
{
    std::vector<Game *> _games;
    class TCPConnection TCPConn;

private:
    void runPlayer(int fd);

public:
    Server();
    ~Server();

    void run();

    /* Called after a request from the client */
    void requestGamesList(int userIndex);
    void requestChangeRole(int userIndex, int roleID);
    void requestChangeMap(int userIndex, std::string mapName);
    void requestAction(int userIndex);
    void requestCreateGame(int userIndex);
    void requestJoinGame(int userIndex, int gameID);
    void requestStartGame(int userIndex);
    void requestMove(int userIndex, std::string moveDir);
    void requestNextLevel(int userIndex);
    void requestLeaveGame(int userIndex);

    Game *getGameFromPlayer(int userIndex);
};

#endif