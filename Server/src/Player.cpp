#include "Player.hpp"
#include "Game.hpp"

Player::Player() : _posX(0), _posY(0), _role(0), _index(0), _connected(true) {}

Player::Player(int index, Game *game) : _index(index), _game(game), _lastDirection("down") {}

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

void Player::setIndex(int index) { _index = index; }

Game *Player::getGame() { return _game; }

void Player::setGame(Game * g) { _game = g; }

int Player::getInGameID() { return _inGameID; }

void Player::setInGameID(int inGameID) { _inGameID = inGameID; }

Player::~Player() {}

bool Player::isConnected() { return _connected; }

void Player::setConnected(bool connected) { _connected = connected; }

std::string Player::getLastDirection() { return _lastDirection; }

void Player::setLastDirection(std::string direction) { _lastDirection = direction; }