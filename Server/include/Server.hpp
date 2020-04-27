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
class TCPConnection;
class Player;

class Server
{
    /* List of games created by the server */
    std::vector<Game *> _games;

    /* Object that handles the TCP connection with the clients */
    TCPConnection TCPConn;

    /* Every players connected to the server */
    Player *_players[MAX_CONNECTION_TCP];

private:

    /** Handles the communication with the users
     * @fd: The index of the player */
    void runPlayer(int fd);

    /** Sends a message to all the players of the corresponding game
     * @game: the game which requires an answer
     * @msg: the message to send to the players in the game
    */
    void broadcastGame(Game *game, std::string msg);

    /** Searches for a game containing the player whose ID is given, and asks to remove it
     * @index: the index of the player to remove */
    void removePlayerFromGame(int index);

public:
    Server();
    ~Server();

    /* Handles the connection from the users */
    void run();

    /* Called after a request from the client */
    void requestGamesList(int userIndex);

    /** Asks the corresponding game to change the role of the player given 
     * @userIndex: the index of the player
     * @roleID: the new role of the player
    */
    void requestChangeRole(int userIndex, int roleID);

    /** Asks the corresponding game to change the current selected map 
     * @userIndex: the index of the player
     * @mapName: the new selected map
    */
    void requestChangeMap(int userIndex, std::string mapName);

    /** Asks the corresponding game to checks if the action of the player is valid,
     * and if so, to executes it 
     * @userIndex: the index of the player
    */
    void requestAction(int userIndex);

    /** Creates a new game with a default map and sends the information to the client requesting the creation
     * @userIndex: the index of the player
    */
    void requestCreateGame(int userIndex);

    /** Asks the corresponding game to add the player to itself 
     * @userIndex: the index of the player
     * @gameID: the ID of the game to join
    */
    void requestJoinGame(int userIndex, int gameID);

    /** Asks the game to initialize itself with the current selected map 
     * @userIndex: the index of the player
    */
    void requestStartGame(int userIndex);

    /** Asks the corresponding game to check if the player can move in this direction,
     * and if so, to handle the movement
     * @userIndex: the index of the player
     * @moveDir: the direction in which the player is moving
    */
    void requestMove(int userIndex, std::string moveDir);

    /** Asks the corresponding game to increase its current level
     * @userIndex: the index of the player */
    void requestNextLevel(int userIndex);

    /** Asks the game to remove the player from itself
     * @userIndex: the index of the player */
    void requestLeaveGame(int userIndex);

    /** Asks the game to return to the lobby
     * @userIndex: the index of the player */
    void requestReturnToLobby(int userIndex);

    /** Asks the game to inform every player that a ping has been sent at the given position
     * @userIndex: the index of the player
     * @posX: the x position of the ping
     * @posY: the y position of the ping*/
    void requestSendPing(int userIndex, int posX, int posY);

    /** Retrieves the game corresponding to the player given
     * @userIndex: the index of the player
     * Returns the game associated to the player */
    Game *getGameFromPlayer(int userIndex);
};

#endif