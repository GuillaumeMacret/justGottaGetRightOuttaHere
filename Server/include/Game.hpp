#ifndef _GAME_H_
#define _GAME_H_

//#define NB_PLAYERS 4

#define EMPTY 0
#define KEY 6
#define BUTTON 71
#define MOVABLE 2
#define BREAKABLE 12
#define WATER 72
#define DOOR 64
#define BRIDGE 28
#define STAIRWAY 29

#define C_NOTHING 0
#define C_WALKABLE 1
#define C_BLOCK 2
#define C_WATER 3

#include <string>
#include <vector>

class Player;
class RSJresource;

struct Tile
{
    int backgroundValue;
    int blockValue;
    int collisionValue; //0: nothing, 1: walkable, 2: block, 3: water
};

struct Point
{
    int posX;
    int posY;
};

struct Block
{
    Point p;
    int value;
};

class Game
{
private:
    //Player *_players[NB_PLAYERS];
    
    /* List of players in the game */
    std::vector<Player *> _players;
    
    /* Array of tiles representing the map state */
    Tile **_grid;

    /* List of 'On' blocks and their position */
    std::vector<Block> _onBlocks;

    /* List of 'Off' blocks and their position */
    std::vector<Block> _offBlocks;

    /* List of stairways and their position */
    std::vector<Block> _stairways;

    /* Position of the 4 tiles representing the lock */
    Point _lockPosition[4];

    /* Boolean that is true if On Blocks are displayed, false otherwise */
    bool _buttonState;

    /* Boolean that is true if the game is finished, false otherwise */
    bool _finished;

    /* Boolean that is true if the game has started, false otherwise */
    bool _started;

    /* Width of the map */
    int _width;

    /* Height of the map */
    int _height;

    /* Number of players in the game */
    int _nbPlayers;

    /* Current level of the party */
    int _currentLevel;

    /* The ID of the game */
    int _gameID;

    /* Number of keys to collect in order to break the lock and complete the level */
    int _nbKeys;

    /* The name of the current map selected by the leader of the party */
    std::string _selectedMap;

    /** Reads the background layer of the selected map, in a JSON format
     * @layerResource: contains the JSON information of the layer */
    void readBackground(RSJresource layerResource);

    /** Reads the block layer of the selected map, in a JSON format
     * @layerResource: contains the JSON information of the layer */
    void readBlocks(RSJresource layerResource);
    
    /** Reads the key layer of the selected map, in a JSON format and which contains the position
     * of the lock and the keys
     * @layerResource: contains the JSON information of the layer */
    void readKey(RSJresource layerResource);

    /** Reads the layer of the selected map which contains the position of the 'On' buttons, in a JSON format
     * @layerResource: contains the JSON information of the layer */
    void readButtonOn(RSJresource layerResource);

    /** Reads the layer of the selected map which contains the position of the 'Off' buttons, in a JSON format
     * @layerResource: contains the JSON information of the layer */
    void readButtonOff(RSJresource layerResource);

    /** Reads the blcollisionock layer of the selected map, in a JSON format
     * @layerResource: contains the JSON information of the layer */
    void readCollision(RSJresource layerResource);

    /** Reads the starting positions of the players in the selected map, in a JSON format
     * @layerResource: contains the JSON information of the layer */
    void readPlayersStartPos(RSJresource layerResource);

    /* Transform a tile into a json object
        * @posX: the X position of the tile
        * @posY: the Y position of the tile
        * @value: the tile ID
        * Returns a string corresponding to the JSON object */ 
    std::string tileToJSON(int posX, int posY, int value);

    /** Checks if the player that requested the action 'Push' can do this action
     * @dir: the direction the player is facing, used to check the object eventually behind a rock
     * @posX: the X position of the tile the player is facing
     * @posY: the Y position of the tile the player is facing
     * Returns the JSON answer to the push action */
    std::string checkPush(std::string dir, int posX, int posY);

    /** Checks if the player that requested the action 'Create' can do this action
     * @dir: the direction faced by the player
     * @posX: the X position of the player
     * @posY: the Y position of the player
     * Returns the JSON answer to the create action */
    std::string checkCreate(int posX, int posY);

    /** Checks if the player that requested the action 'Activate' can do this action
     * @dir: the direction faced by the player
     * @posX: the X position of the player
     * @posY: the Y position of the player
     * Returns the JSON answer to the activate action */
    std::string checkActivate(int posX, int posY);

    /** Checks if the player that requested the action 'Break' can do this action
     * @dir: the direction faced by the player
     * @posX: the X position of the player
     * @posY: the Y position of the player
     * Returns the JSON answer to the break action */
    std::string checkBreak(int posX, int posY);

public:
    enum Roles
    {
        PUSH = 0,
        CREATE = 1,
        ACTIVATE = 2,
        BREAK = 3
    };

    /** Constructor of the game
     * @gameID: the ID of the game
     * @selectedMap: the current map selected by the player */
    Game(int gameID, std::string selectedMap = "map");

    /* Function called by the server when a player asks to move
        * @playerID: ID of the requesting player
        * @direction: up, down, left or right
        * Returns the blocks changed after the move (keys or lock) */
    std::string movePlayer(int playerID, std::string direction);

    /** Searches for the tile the player is facing, and call the associated function checking its action is valid
     * @playerID: the ID of the requesting player
     * Returns the JSON answer to the action request */
    std::string doActionPlayer(int playerID);

    /** Adds a player to the game if possible (not enough players or a player is disconnected)
     * @p: The player to add to the game
     * Returns true if the player has been added, false otherwise */
    bool addPlayer(Player *p);

    /** Disconnects a player from the game, without removing it
     * @playerIndex: the player of the disconnected player */
    void disconnectPlayer(int playerIndex);

    /** Checks if the game is empty or not
     * Returns true if no player is connected to the game, false otherwise */
    bool isPlayerListEmpty();

    /** Checks if the game is finished
     * Returns true is the game is finished, false otherwise
     * */
    bool getFinished();

    /** Changes the selected map
     * @mapName: the newly selected map */
    void changeMap(std::string mapName);

    /** Changes the role of a player
     * @roleID: the role to assign
     * @playerID: the player changing its role */
    void changeRole(int roleID, int playerID);

    /* Returns the ID of the game */
    int getGameID();

    /* Increases the game level */
    void increaseLevel();

    /* Retrieves the information of the selected map, with the format given by Tiled */
    void readMap();

    /* Restarts the game, with its default values */
    void resetGame();

    /* Returns the name of the selected map */
    std::string getMapName();

    /* Transforms the map in a JSON format that can be read by the client */
    std::string getMapToJSON();

    /* Transforms the players information in a JSON format that can be read by the client */
    std::string getPlayersToJSON();

    /* Returns the list of the players in the game */
    std::vector<Player *> getPlayers();

    /* Returns the number of players connected to the game */
    int getNbConnectedPlayers();

    /* Returns true if the game has started, false otherwise */
    bool getStarted();

    /* Changes the game state */
    void setStarted(bool);


    ~Game();
};

#endif