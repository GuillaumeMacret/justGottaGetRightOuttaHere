#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "Game.hpp"

class Player
{
    /* X position of the player */
    int _posX;
    /* Y position of the player */
    int _posY;
    /* Role ID of the player */
    int _role;
    /* The index of the player in the server */
    int _index;
    /* The game the player is in */
    class Game *_game;
    /* The ID of the player in the game */
    int _inGameID;
    /* Indicates if the player is currently connected to the server */
    bool _connected;
    /* Indicates if the player can use its secondary action */
    bool _secondaryAction;
    /* Indicates if the player is standing on a lock, used to check victory condition */
    bool _onLock;
    /* The direction the player is facing */
    std::string _lastDirection;
    /* The previous state of the tile the player stands on.
     * This allows the tile to go back to its previous state after the player has moved.
     * For instance, it is not possible to push an object on a tile where there is a player,
     * But once the player has moved, it is possible again. In our case, this is more effective than
     * Checking if the tile is empty or not. */
    int _lastCollisionType;
    /* The dummy placed by the Pumpkin if possible, in order to teleport to it */
    Point *_dummy;

public:
    Player();
    Player(int index, class Game *game = nullptr);
    Player(Player*);

    /* Getter for the X position */
    int getPosX();
    /* Getter for the Y position */
    int getPosY();
    /* Sets the position of the player to a new one */
    void setPos(int posX, int posY);
    /* Getter for the role ID of th player */
    int getRole();
    /* Changes the role of the player */
    void setRole(int role);
    /* Getter for the index of the player in the server */
    int getIndex();
    /* Changes the player index in the server */
    void setIndex(int index);
    /* Getter for the previous state of the tile the player stands on */
    int getLastCollisionType();
    /* Sets the previous state of the tile the player stands on to a new one */
    void setLastCollisionType(int type);
    /* Getter for the game the player is in */
    class Game *getGame();
    /* Changes the game the player is in */
    void setGame(Game *g);
    /* Getter for the in-game ID */
    int getInGameID();
    /* Changes the in-game ID */
    void setInGameID(int inGameID);
    /* Getter to know if the player is connected to the server */
    bool isConnected();
    /* Changes the connected status of the player */
    void setConnected(bool connected);
    /* Getter for the secondary action boolean of the player */
    bool hasSecondaryAction();
    /* Enables the secondary action of the player, or disables if the game restarts */
    void setSecondaryAction(bool secondaryAction);
    /* Indicates if the player stands on a lock */
    bool isOnLock();
    /* Sets the onLock state of the player */
    void setOnLock(bool onLock);
    /* Getter for the direction the player is facing */
    std::string getLastDirection();
    /* Sets the direction the player is facing to a new one */
    void setLastDirection(std::string direction);
    /* Getter for the dummy of the player */
    Point *getDummy();
    /* Sets the dummy of the player */
    Point *setDummy();
    void teleport();
    void copy(Player *);

    ~Player();
};

#endif