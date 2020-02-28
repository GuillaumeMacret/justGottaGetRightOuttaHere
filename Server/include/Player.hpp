#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player
{
    int _posX;
    int _posY;
    int _role;
    int _index;

public:
    Player();
    Player(int index);

    int getPosX();
    void setPosY(int posX);
    int getPosY();
    void setPos(int posX, int posY);
    int getRole();
    void setRole(int role);
    int getIndex();

    ~Player();
};

#endif