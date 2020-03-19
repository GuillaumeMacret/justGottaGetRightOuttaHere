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
        _players[playerID]->setLastDirection(direction);
    }

    return _players[playerID];
}

std::string Game::doActionPlayer(int playerID)
{
    std::string res = "[";
    Player *p = _players[playerID];
    int posX = p->getPosX(), posY = p->getPosY();
    if (p->getLastDirection() == "up")
    {
        --posY;
    }
    else if (p->getLastDirection() == "down")
    {
        ++posY;
    }
    else if (p->getLastDirection() == "left")
    {
        --posX;
    }
    else if (p->getLastDirection() == "right")
    {
        ++posX;
    }

    //TODO check if tile [posX, posY] correspond to p->getRole()
    res += "{\"xPos\": " + posX;
    res += ",\"yPos\": " + posY;
    res += ",\"value\": " + _grid[posX][posY];
    res += "}";

    res += "]";
    return res;
}

bool Game::addPlayer(Player *p)
{
    if (_players.size() < 4)
    {
        p->setInGameID(_players.size());
        _players.push_back(p);
        return true;
    }
    for (auto it : _players)
    {
        if (!it->isConnected())
        {
            it->setIndex(p->getIndex());
            p = it;
            return true;
        }
    }
    return false;
}

void Game::disconnectPlayer(int playerIndex)
{
    for (auto it = _players.begin(); it != _players.end(); ++it)
    {
        if ((*it)->getIndex() == playerIndex)
        {
            (*it)->setConnected(false);
        }
    }
}

bool Game::isPlayerListEmpty()
{
    for (auto it = _players.begin(); it != _players.end(); ++it)
    {
        if ((*it)->isConnected())
        {
            return false;
        }
    }
    return true;
}

void Game::changeMap(std::string mapName)
{
    _selectedMap = mapName;
}

void Game::changeRole(int roleID, int playerID)
{
    _players[playerID]->setRole(roleID);
}

void Game::increaseLevel()
{
    ++_currentLevel;
}

int Game::getGameID() { return _gameID; }

std::string Game::getMapName() { return _selectedMap; }

std::string Game::getMapToJSON()
{
    //TO DO: read map from file and add it to mapJSON as we read it

    std::string mapJSON = "\"Blocks\":[";
    for (int i = 0; i < _height; ++i)
    {
        if(i) mapJSON += ',';
        mapJSON += '[';
        for (int j = 0; j < _width; ++j)
        {   
            if(j) mapJSON += ',';
            mapJSON += _grid[i][j];
        }
        mapJSON += ']';
    }
    mapJSON += "],\"Objects\":[";
    for(size_t i = 0; i < _objects.size(); ++i) {
        if(i) mapJSON += ',';
        mapJSON += "{xPos:" + _objects[i]->posX;
        mapJSON += ",yPos:" + _objects[i]->posY;
        mapJSON += ",value:" + _objects[i]->value;
        mapJSON += '}';
    }
    mapJSON += ']';
    return mapJSON;
}

std::string Game::getPlayersToJSON()
{
    std::string playersJSON = "\"Players\":[";
    size_t i = 0;
    for (Player *p : _players)
    {
        playersJSON += "{\"xPos\":" + p->getPosX();
        playersJSON += ",\"yPos\":" + p->getPosY();
        playersJSON += "}";
        if (i != _players.size() - 1)
            playersJSON += ',';
        ++i;
    }
    playersJSON += ']';
    return playersJSON;
}

std::vector<Player *> Game::getPlayers() { return _players; }

Game::~Game()
{
    for (Player *p : _players)
        delete p;

    for (int i = 0; i < _height; ++i)
    {
        if (_grid[i])
        {
            delete[] _grid[i];
        }
    }
}