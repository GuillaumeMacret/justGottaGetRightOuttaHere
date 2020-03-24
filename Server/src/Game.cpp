#include "Game.hpp"
#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "RSJParser.tcc"

Game::Game(int gameID, std::string selectedMap) : _buttonState(false), _nbPlayers(0), _currentLevel(0), _gameID(gameID), _nbKeys(0), _selectedMap(selectedMap) {}

std::string Game::movePlayer(int playerID, std::string direction)
{
    int newPosX = _players[playerID]->getPosX(), newPosY = _players[playerID]->getPosY();
    int posX = newPosX, posY = newPosY;
    std::string changes = "";
    if (direction == "up")
    {
        --newPosY;
    }
    else if (direction == "down")
    {
        ++newPosY;
    }
    else if (direction == "left")
    {
        --newPosX;
    }
    else if (direction == "right")
    {
        ++newPosX;
    }
    std::cout << "LastPos: [" << posX << "," << posY << "] --- NewPos: [" << newPosX << "," << newPosY << "]" << std::endl;
    if (newPosX >= 0 && newPosX < _width && newPosY >= 0 && newPosY < _height)
    {
        std::cout << "Positions valides"<<std::endl;
        if (_grid[newPosY][newPosX].collisionValue < C_BLOCK && (posX != newPosX || posY != newPosY))
        {
            std::cout << "No collision" << std::endl;
            Player *p = _players[playerID];
            _grid[posY][posX].collisionValue = p->getLastCollisionType();

            //Player moved and now stands on a stairway
            if (_grid[newPosY][newPosX].backgroundValue == STAIRWAY)
            {
                int index = 0;
                for (Block sw : _stairways)
                {
                    ++index;
                    if (sw.p.posX == newPosX && sw.p.posY == newPosY)
                    {
                        break;
                    }
                }
                Block b = _stairways[index % _stairways.size()];
                p->setPos(b.p.posX, b.p.posY);
            }
            else
            {
                p->setPos(newPosX, newPosY);

                //Player stands on a key
                if (_grid[newPosY][newPosX].blockValue == KEY)
                {
                    _grid[newPosY][newPosX].blockValue = EMPTY;
                    _grid[newPosY][newPosX].collisionValue = C_NOTHING;
                    p->setLastCollisionType(C_NOTHING);
                    changes += tileToJSON(posX, posY, EMPTY);
                    --_nbKeys;
                    if (!_nbKeys)
                    {
                        //change lock to empty
                        int i = 0;
                        for (Point point : _lockPosition)
                        {
                            if (i)
                                changes += ',';
                            _grid[point.posY][point.posX].blockValue += 2;
                            _grid[point.posY][point.posX].collisionValue = C_NOTHING;
                            changes += tileToJSON(newPosX, newPosY, _grid[point.posY][point.posX].blockValue + 2);
                            ++i;
                        }
                    }
                }
            }
            p->setLastCollisionType(_grid[newPosY][newPosX].collisionValue);
        }
        std::cout << "Collision value: " << _grid[newPosX][newPosY].collisionValue << std::endl;
    }
    _players[playerID]->setLastDirection(direction);
    return changes;
}

std::string Game::tileToJSON(int posX, int posY, int value)
{
    std::string res = "{\"xPos\": " + std::to_string(posX);
    res += ",\"yPos\": " + std::to_string(posY);
    res += ",\"value\": " + std::to_string(value);
    res += "}";
    return res;
}

std::string Game::checkPush(std::string dir, int posX, int posY)
{
    std::string res = "";
    int pushX = posX, pushY = posY;
    if (_grid[posY][posX].blockValue == MOVABLE)
    {
        if (dir == "up")
        {
            --pushY;
        }
        else if (dir == "down")
        {
            ++pushY;
        }
        else if (dir == "left")
        {
            --pushX;
        }
        else if (dir == "right")
        {
            ++pushX;
        }

        if (pushX >= 0 && pushX < _width && pushY >= 0 && pushY < _height && _grid[pushY][pushX].collisionValue == C_NOTHING)
        {
            _grid[posY][posX].blockValue = EMPTY;
            _grid[posY][posX].collisionValue = C_NOTHING;
            _grid[pushY][pushX].blockValue = MOVABLE;
            _grid[pushY][pushX].collisionValue = C_BLOCK;
            res += tileToJSON(posX, posY, EMPTY);
            res += ',';
            res += tileToJSON(pushX, pushY, MOVABLE);
        }
    }

    return res;
}

std::string Game::checkCreate(int posX, int posY)
{
    std::string res = "";
    if (_grid[posY][posX].blockValue == WATER)
    {
        _grid[posY][posX].blockValue = BRIDGE;
        _grid[posY][posX].collisionValue = C_NOTHING;
        res += tileToJSON(posX, posY, BRIDGE);
    }
    return res;
}

std::string Game::checkActivate(int posX, int posY)
{
    std::string res = "";

    if (_grid[posY][posX].blockValue == BUTTON)
    {
        int i = 0;
        if (_buttonState)
        {
            for (Block ofb : _onBlocks)
            {
                if (i)
                    res += ',';
                _grid[ofb.p.posY][ofb.p.posX].blockValue = ofb.value;
                res += tileToJSON(ofb.p.posX, ofb.p.posY, ofb.value);
                ++i;
            }
            for (Block ofb : _offBlocks)
            {
                res += ',';
                _grid[ofb.p.posY][ofb.p.posX].blockValue = EMPTY;
                res += tileToJSON(ofb.p.posX, ofb.p.posY, EMPTY);
            }
        }
        else
        {
            for (Block ofb : _onBlocks)
            {
                if (i)
                    res += ',';
                _grid[ofb.p.posY][ofb.p.posX].blockValue = EMPTY;
                res += tileToJSON(ofb.p.posX, ofb.p.posY, EMPTY);
                ++i;
            }
            for (Block ofb : _offBlocks)
            {
                res += ',';
                _grid[ofb.p.posY][ofb.p.posX].blockValue = ofb.value;
                res += tileToJSON(ofb.p.posX, ofb.p.posY, ofb.value);
            }
        }
        _buttonState = !_buttonState;
    }

    return res;
}

std::string Game::checkBreak(int posX, int posY)
{
    std::string res = "";
    if (_grid[posY][posX].blockValue == BREAKABLE)
    {
        _grid[posY][posX].blockValue = EMPTY;
        _grid[posY][posX].collisionValue = C_NOTHING;
        res += tileToJSON(posX, posY, EMPTY);
    }
    return res;
}

std::string Game::doActionPlayer(int playerID)
{
    std::string res = "[";
    Player *p = _players[playerID];
    int posX = p->getPosX(), posY = p->getPosY();
    std::string dir = p->getLastDirection();
    if (dir == "up")
    {
        --posY;
    }
    else if (dir == "down")
    {
        ++posY;
    }
    else if (dir == "left")
    {
        --posX;
    }
    else if (dir == "right")
    {
        ++posX;
    }

    if (posX >= 0 && posX < _width && posY >= 0 && posY < _height)
    {
        switch (p->getRole())
        {
        case PUSH:
            res += checkPush(dir, posX, posY);
            break;
        case CREATE:
            res += checkCreate(posX, posY);
            break;
        case ACTIVATE:
            res += checkActivate(posX, posY);
            break;
        case BREAK:
            res += checkBreak(posX, posY);
            break;
        }
    }

    res += "]";
    return res;
}

bool Game::addPlayer(Player *p)
{
    if (_players.size() < 4)
    {
        p->setGame(this);
        p->setInGameID(_players.size());
        int role = 0;
        for (auto it : _players)
        {
            if (role == it->getRole())
                ++role;
        }
        p->setRole(role);
        _players.push_back(p);
        return true;
    }
    for (auto it : _players)
    {
        if (!it->isConnected())
        {
            it->setIndex(p->getIndex());
            it->setGame(this);
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

void Game::readBackground(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value)
        {
            _grid[j][i] = Tile{value, EMPTY, C_NOTHING};
            if (value == STAIRWAY)
            {
                _stairways.push_back(Block{Point{i, j}, value});
            }
        }
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readBlocks(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value && value)
            _grid[j][i].blockValue = value;
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readKey(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0, lockIndex = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value && value)
        {
            if (value == KEY)
            {
                ++_nbKeys;
            }
            else
            {
                _lockPosition[lockIndex++] = Point{i, j};
            }
            _grid[j][i].blockValue = value;
        }
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readButtonOn(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value && value)
        {
            if (value != BUTTON)
            {
                Point p = Point{i, j};
                _onBlocks.push_back(Block{p, value});
            }
            else
                _grid[j][i].blockValue = value;
        }
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readButtonOff(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value && value)
        {
            if (value != BUTTON)
            {
                Point p = Point{i, j};
                _offBlocks.push_back(Block{p, value});
            }
            _grid[j][i].blockValue = value;
        }
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readCollision(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value)
            _grid[j][i].collisionValue = value;
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readPlayersStartPos(RSJresource layerResource)
{
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1, layerString.length() - 2);

    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value && value != 0 && value - 1 < (int)_players.size())
            _players[value - 1]->setPos(i, j);
        tmp = "";
        ++i;
        if (i % _width == 0)
        {
            ++j;
            i = 0;
        }
    }
}

void Game::readMap()
{
    std::ifstream f("maps/" + _selectedMap + ".json");
    if (f.good())
    {
        std::string levelString = "";
        for (std::string line; std::getline(f, line);)
        {
            levelString += line;
        }
        RSJresource json_file_resource(levelString);

        _width = json_file_resource["width"].as<int>();
        _height = json_file_resource["height"].as<int>();

        _grid = new Tile *[_height];
        for (int i = 0; i < _height; ++i)
        {
            _grid[i] = new Tile[_width];
        }

        for (auto it = json_file_resource["layers"].as_array().begin(); it != json_file_resource["layers"].as_array().end(); ++it)
        {
            RSJresource layerResource = it->as<RSJresource>();

            if (layerResource["name"].as<std::string>() == "Sol")
            {
                readBackground(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == "Collision")
            {
                readCollision(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == "Cle")
            {
                readKey(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == "Interrupteur 2")
            {
                readButtonOn(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == "Interrupteur 1")
            {
                readButtonOff(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == "Joueur")
            {
                readPlayersStartPos(layerResource);
            }
            else
            {
                readBlocks(layerResource);
            }
        }
    }
}

std::string Game::getMapToJSON()
{
    readMap();
    //bool firstObject = true;
    std::string mapJSON = "\"Name\":\"" + _selectedMap;
    mapJSON += "\", \"Blocks\":[";
    std::string objectsJSON = "\"Objects\":[";
    for (int i = 0; i < _height; ++i)
    {
        if (i)
        {
            mapJSON += ',';
            objectsJSON += ',';
        }
        mapJSON += '[';
        objectsJSON += '[';
        for (int j = 0; j < _width; ++j)
        {
            if (j)
            {
                mapJSON += ',';
                objectsJSON += ',';
            }
            mapJSON += std::to_string(_grid[i][j].backgroundValue);
            objectsJSON += std::to_string(_grid[i][j].blockValue);
            /*if (_grid[i][j].blockValue)
            {
                if (firstObject)
                {
                    firstObject = false;
                }
                else
                {
                    objectsJSON += ',';
                }

                objectsJSON += tileToJSON(j, i, _grid[i][j].blockValue);
            }*/
        }
        mapJSON += ']';
        objectsJSON += ']';
    }
    mapJSON += "],";
    objectsJSON += "],";
    mapJSON += objectsJSON;
    return mapJSON;
}

std::string Game::getPlayersToJSON()
{
    std::string playersJSON = "\"Players\":[";
    size_t i = 0;
    for (Player *p : _players)
    {
        playersJSON += "{\"xPos\":" + std::to_string(p->getPosX());
        playersJSON += ",\"yPos\":" + std::to_string(p->getPosY());
        playersJSON += "}";
        if (i != _players.size() - 1)
            playersJSON += ',';
        ++i;
    }
    playersJSON += ']';
    return playersJSON;
}

std::vector<Player *> Game::getPlayers() { return _players; }

int Game::getNbConnectedPlayers()
{
    int count = 0;
    for (auto it : _players)
    {
        if (it->isConnected())
            ++count;
    }
    return count;
}

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