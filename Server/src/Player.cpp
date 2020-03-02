#include "Player.hpp"
#include "Game.hpp"

Player::Player() : _posX(0), _posY(0), _role(0), _index(0) {}

Player::Player(int index, Game *game) : _index(index), _game(game) {}

int Player::getPosX() { return _posX; }

int Player::getPosY() { return _posY; }

void Player::setPos(int posX, int posY)
{
    _posX = posX;
    _posY = posY;
}

int Player::getRole() { return _role; }

void Player::setRole(int role) { _role = role; }

int Player::getIndex() { return _index; }

Game *Player::getGame() { return _game; }

Player::~Player() {}