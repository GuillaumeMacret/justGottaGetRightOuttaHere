#ifndef _GAME_H_
#define _GAME_H_

//#define NB_PLAYERS 4

#include <string>
#include <vector>

class Player;

class Game
{
private:
    //Player *_players[NB_PLAYERS];
    std::vector<Player *> _players;
    char **_grid;
    int _width;
    int _height;
    int _nbPlayers;
    int _currentLevel;
    int _gameID;
    std::string _selectedMap;

public:
    enum class Roles
    {
        PUSH,
        CREATE,
        ACTIVATE,
        BREAK
    };

    Game(int gameID);

    /* Function called by the server when a player asks to move
        * @playerID: ID of the requesting player
        * @direction: up, down, left or right
        * Returns the player with its new position. */
    Player *movePlayer(int playerID, std::string direction);

    void addPlayer(Player *p);
    void removePlayer(int playerIndex);
    void changeMap(std::string mapName);
    void changeRole(int roleID, int playerID);
    int getGameID();
    void increaseLevel();
    std::string getMapName();
    std::vector<Player *> getPlayers();

    ~Game();
};

#endif