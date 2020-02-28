#include "Game.hpp"
#include "Player.hpp"

Game::Game(int gameID) : _nbPlayers(0), _currentLevel(0), _gameID(gameID) {}

Player *Game::movePlayer(int playerID, std::string direction)
{
    int posX = _players[playerID]->getPosX(), posY = _players[playerID]->getPosY();
    if (direction == "up")
    {
        --posY;
    }
    else if (direction == "down")
    {
        ++posY;
    }
    else if (direction == "left")
    {
        --posX;
    }
    else if (direction == "right")
    {
        ++posX;
    }

    if (posX >= 0 && posX < _width && posY >= 0 && posY < _height)
    {
        //TO-DO: check if _grid[posY][posX] is walkable
        _players[playerID]->setPos(posX, posY);
    }

    return _players[playerID];
}

void Game::addPlayer(Player *p) {
    _players.push_back(p);
}

void Game::removePlayer(int playerIndex) {
    for(auto it = _players.begin(); it != _players.end(); ++it) {
        if((*it)->getIndex() == playerIndex) {
            _players.erase(it);
        }
    }
}

void Game::changeMap(std::string mapName)
{
    _selectedMap = mapName;
}

void Game::changeRole(int roleID, int playerID)
{
    //TO DO
    _players[playerID]->setRole(roleID);
}

void Game::increaseLevel() {
    ++_currentLevel;
}

int Game::getGameID() { return _gameID; }

std::string Game::getMapName() { return _selectedMap; }

std::vector<Player *> Game::getPlayers() { return _players; }

Game::~Game()
{
    for(Player *p : _players) delete p;

    for (int i = 0; i < _height; ++i)
    {
        if (_grid[i])
        {
            delete[] _grid[i];
        }
    }
}