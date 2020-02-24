#include "Game.hpp"
#include "Player.hpp"

Game::Game() : _nbPlayers(0), _currentLevel(0) {}

Player *Game::movePlayer(int playerID, std::string direction) {
    int posX = _players[playerID]->getPosX(), posY = _players[playerID]->getPosY();
    if(direction == "up") {
        --posY;
    } else if(direction == "down") {
        ++posY;
    } else if(direction == "left") {
        --posX;
    } else if(direction == "right") {
        ++posX;
    }

    if(posX >= 0 && posX < _width && posY >= 0 && posY < _height) {
        //TO-DO: check if _grid[posY][posX] is walkable
        _players[playerID]->setPos(posX, posY);
    }

    return _players[playerID];
}

void Game::changeMap(std::string mapName) {
    _selectedMap = mapName;
}

void Game::changeRole(int roleID, int playerID) {
    //TO DO
    _players[playerID]->setRole(roleID);
}

Game::~Game() {
    for(int i = 0; i < NB_PLAYERS; ++i) {
        if(_players[i]) {
            delete _players[i];
        }
    }

    for(int i = 0; i < _height; ++i) {
        if(_grid[i]) {
            delete[] _grid[i];
        }
    }
}