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

void Game::addPlayer(Player *p)
{
    p->setInGameID(_players.size());
    _players.push_back(p);
}

void Game::removePlayer(int playerIndex)
{
    for (auto it = _players.begin(); it != _players.end(); ++it)
    {
        if ((*it)->getIndex() == playerIndex)
        {
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
    std::string mapJSON = "Level:[";
    for (int i = 0; i < _height; ++i)
    {
        mapJSON += "[";
        for (int j = 0; j < _width; ++j)
        {
            mapJSON += _grid[i][j];
        }
        mapJSON += "],";
    }
    mapJSON += "],";
    return mapJSON;
}

std::string Game::getPlayersToJSON()
{
    std::string playersJSON = "Players:[";
    size_t i = 0;
    for (Player *p : _players)
    {
        playersJSON += "{xPos:" + p->getPosX();
        playersJSON += ",yPos:" + p->getPosY();
        playersJSON += "}";
        if (i != _players.size() - 1)
            playersJSON += ",";
        ++i;
    }
    playersJSON += "]";
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