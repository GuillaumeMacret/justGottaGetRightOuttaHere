#include "Player.hpp"

Player::Player() : _posX(0), _posY(0), _role(0) {}

Player::Player(int posX, int posY, int role) : _posX(posX), _posY(posY) {}

int Player::getPosX() { return _posX; }

int Player::getPosY() { return _posY; }

void Player::setPos(int posX, int posY)
{
    _posX = posX;
    _posY = posY;
}

int Player::getRole() { return _role; }

void Player::setRole(int role) { _role = role; }

Player::~Player() {}