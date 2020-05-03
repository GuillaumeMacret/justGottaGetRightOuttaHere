#include "Player.hpp"
#include "Game.hpp"

Player::Player() : _posX(0), _posY(0), _role(0), _index(0), _connected(true), _secondaryAction(false), _onLock(false), _ready(false), _lastCollisionType(0), _dummy(nullptr) {}

Player::Player(int index, Game *game) : _posX(0), _posY(0), _role(0), _index(index), _game(game), _connected(true), _lastDirection("down") {}

Player::Player(Player *p)
{
    copy(p);
}

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

int Player::getLastCollisionType() { return _lastCollisionType; }

void Player::setLastCollisionType(int type) { _lastCollisionType = type; }

Game *Player::getGame() { return _game; }

void Player::setGame(Game *g) { _game = g; }

int Player::getInGameID() { return _inGameID; }

void Player::setInGameID(int inGameID) { _inGameID = inGameID; }

Player::~Player() {}

bool Player::isConnected() { return _connected; }

void Player::setConnected(bool connected) { _connected = connected; }

bool Player::hasSecondaryAction() { return _secondaryAction; }

void Player::setSecondaryAction(bool secondaryAction) { _secondaryAction = secondaryAction; }

bool Player::isOnLock() { return _onLock; }

void Player::setOnLock(bool onLock) { _onLock = onLock; }

bool Player::getReadyStatus() { return _ready; }

void Player::setReadyStatus(bool readyStatus) { _ready = readyStatus; }

std::string Player::getLastDirection() { return _lastDirection; }

void Player::setLastDirection(std::string direction) { _lastDirection = direction; }

Point *Player::getDummy() { return _dummy; }

Point *Player::setDummy()
{
    _dummy = new Point();
    _dummy->posX = _posX;
    _dummy->posY = _posY;
    return _dummy;
}

void Player::deleteDummy()
{
    if (_dummy)
    {
        delete _dummy;
        _dummy = nullptr;
    }
}

void Player::teleport()
{
    setPos(_dummy->posX, _dummy->posY);
    deleteDummy();
}

void Player::copy(Player *p)
{
    _posX = p->_posX;
    _posY = p->_posY;
    _role = p->_role;
    _index = p->_index;
    _game = p->_game;
    _inGameID = p->_inGameID;
    _connected = p->_connected;
    _secondaryAction = p->_secondaryAction;
    _onLock = p->_onLock;
    _lastDirection = p->_lastDirection;
    _lastCollisionType = p->_lastCollisionType;
    _dummy = p->_dummy;
}