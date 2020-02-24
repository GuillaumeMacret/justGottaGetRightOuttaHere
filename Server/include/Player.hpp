#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player {
        int _posX;
        int _posY;
        int _role;

    public:
        Player();
        Player(int posX, int posY, int role);

        int getPosX();
        void setPosY(int posX);
        int getPosY();
        void setPos(int posX, int posY);
        int getRole();
        void setRole(int role);

        ~Player();
};

#endif