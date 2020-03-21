#ifndef _GAME_H_
#define _GAME_H_

//#define NB_PLAYERS 4

#define KEY 6
#define BUTTON 71

#include <string>
#include <vector>

class Player;
class RSJresource;

struct Tile {
    int backgroundValue;
    int blockValue;
    int collisionValue; //0: nothing, 1: walkable, 2: block, 3: water
};

struct Point {
    int posX;
    int posY;
};

class Game
{
private:
    //Player *_players[NB_PLAYERS];
    std::vector<Player *> _players;
    Tile **_grid;
    std::vector<Point> _onBlocks;
    std::vector<Point> _offBlocks;
    Point _lockPosition[4];
    bool _buttonState;
    int _width;
    int _height;
    int _nbPlayers;
    int _currentLevel;
    int _gameID;
    int _nbKeys;
    std::string _selectedMap;

    void readBackground(RSJresource layerResource);
    void readBlocks(RSJresource layerResource);
    void readKey(RSJresource layerResource);
    void readButtonOn(RSJresource layerResource);
    void readButtonOff(RSJresource layerResource);
    void readCollision(RSJresource layerResource);

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
    std::string doActionPlayer(int playerID);

    bool addPlayer(Player *p);
    void disconnectPlayer(int playerIndex);
    bool isPlayerListEmpty();
    void changeMap(std::string mapName);
    void changeRole(int roleID, int playerID);
    int getGameID();
    void increaseLevel();
    void readMap();
    std::string getMapName();
    std::string getMapToJSON();
    std::string getPlayersToJSON();
    std::vector<Player *> getPlayers();

    ~Game();
};

#endif