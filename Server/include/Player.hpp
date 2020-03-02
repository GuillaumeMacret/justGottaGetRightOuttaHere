#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player
{
    int _posX;
    int _posY;
    int _role;
    int _index;
    class Game *_game;

public:
    Player();
    Player(int index, class Game *game);

    int getPosX();
    void setPosY(int posX);
    int getPosY();
    void setPos(int posX, int posY);
    int getRole();
    void setRole(int role);
    int getIndex();
    class Game *getGame();

    ~Player();
};

#endif