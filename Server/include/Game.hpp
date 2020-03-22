#ifndef _GAME_H_
#define _GAME_H_

//#define NB_PLAYERS 4

#define EMPTY 0
#define KEY 6
#define BUTTON 71
#define MOVABLE 2
#define BREAKABLE 12
#define WATER 72
#define BRIDGE 28

#define C_NOTHING 0
#define C_WALKABLE 1
#define C_BLOCK 2
#define C_WATER 3

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

struct OnOffBlock {
    Point p;
    int value;
};

class Game
{
private:
    //Player *_players[NB_PLAYERS];
    std::vector<Player *> _players;
    Tile **_grid;
    std::vector<OnOffBlock> _onBlocks;
    std::vector<OnOffBlock> _offBlocks;
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

    std::string tileToJSON(int posX, int posY, int value);

    std::string checkPush(std::string dir, int posX, int posY);
    std::string checkCreate(int posX, int posY);
    std::string checkActivate(int posX, int posY);
    std::string checkBreak(int posX, int posY);

public:
    enum Roles
    {
        PUSH = 0,
        CREATE = 1,
        ACTIVATE = 2,
        BREAK = 3
    };

    Game(int gameID);

    /* Function called by the server when a player asks to move
        * @playerID: ID of the requesting player
        * @direction: up, down, left or right
        * Returns the blocks changed after the moe (keys or lock) */
    std::string movePlayer(int playerID, std::string direction);
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