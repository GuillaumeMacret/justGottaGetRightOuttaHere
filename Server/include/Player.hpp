#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>

class Player
{
    int _posX;
    int _posY;
    int _role;
    int _index;
    class Game *_game;
    int _inGameID;
    bool _connected;
    bool _secondaryAction;
    std::string _lastDirection;
    int _lastCollisionType;

public:
    Player();
    Player(int index, class Game *game = nullptr);

    int getPosX();
    int getPosY();
    void setPos(int posX, int posY);
    int getRole();
    void setRole(int role);
    int getIndex();
    void setIndex(int index);
    int getLastCollisionType();
    void setLastCollisionType(int type);
    class Game *getGame();
    void setGame(Game *g);
    int getInGameID();
    void setInGameID(int inGameID);
    bool isConnected();
    void setConnected(bool connected);
    bool hasSecondaryAction();
    void setSecondaryAction(bool secondaryAction);
    std::string getLastDirection();
    void setLastDirection(std::string direction);

    ~Player();
};

#endif