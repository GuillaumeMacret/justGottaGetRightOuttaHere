#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player
{
    int _posX;
    int _posY;
    int _role;
    int _index;
    class Game *_game;
    int _inGameID;
    bool _connected;

public:
    Player();
    Player(int index, class Game *game = nullptr);

    int getPosX();
    void setPosY(int posX);
    int getPosY();
    void setPos(int posX, int posY);
    int getRole();
    void setRole(int role);
    int getIndex();
    void setIndex(int index);
    class Game *getGame();
    int getInGameID();
    void setInGameID(int inGameID);
    bool isConnected();
    void setConnected(bool connected);

    ~Player();
};

#endif