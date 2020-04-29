#include "Game.hpp"
#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "RSJParser.tcc"

Game::Game(int gameID, std::string selectedMap) : _buttonState(true), _finished(false), _started(false), _nbPlayers(0), _currentLevel(0), _gameID(gameID), _nbKeys(0), _selectedMap(selectedMap) {}

void Game::enableSecondaryAction(int roleID)
{
    for (Player *p : _players)
    {
        if (p->getRole() == roleID)
        {
            p->setSecondaryAction(true);
        }
    }
}

bool Game::checkOnObject(int tileValue)
{
    switch (tileValue)
    {
    case SWORD:
        enableSecondaryAction(BREAK);
        return true;
    case MONEY:
        enableSecondaryAction(CLIMB);
        return true;
    case BOOK:
        enableSecondaryAction(CREATE);
        return true;
    case DUMMY:
        enableSecondaryAction(ACTIVATE);
        return true;
    case DIADEM:
        enableSecondaryAction(PUSH);
        return true;
    }
    return false;
}

std::string Game::movePlayer(int playerID, std::string direction)
{
    Player *p = _players[playerID];
    int newPosX = p->getPosX(), newPosY = p->getPosY();
    int posX = newPosX, posY = newPosY;
    bool _foundLock = false;
    std::string changes = "";

    // Retrieves the position the player is facing
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

    // Checks if the player tries to go outside of the map
    if (newPosX >= 0 && newPosX < _width && newPosY >= 0 && newPosY < _height)
    {
        // Checks if the player can move on the tile it is facing
        if (_grid[newPosY][newPosX].collisionValue < C_BLOCK && (posX != newPosX || posY != newPosY))
        {
            _grid[posY][posX].collisionValue = p->getLastCollisionType();

            // Player moved and now stands on a stairway
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
                if (checkOnObject(_grid[newPosY][newPosX].blockValue))
                {
                    _grid[newPosY][newPosX].blockValue = EMPTY;
                    _grid[newPosY][newPosX].collisionValue = C_NOTHING;
                    changes += tileToJSON(newPosX, newPosY, EMPTY);
                }
                else
                {
                    //Player stands on a key
                    if (_grid[newPosY][newPosX].blockValue == KEY)
                    {
                        _grid[newPosY][newPosX].blockValue = EMPTY;
                        _grid[newPosY][newPosX].collisionValue = C_NOTHING;
                        changes += tileToJSON(newPosX, newPosY, EMPTY);
                        --_nbKeys;
                        if (!_nbKeys)
                        {
                            //change lock to empty
                            for (Point point : _lockPosition)
                            {
                                changes += ',';
                                _grid[point.posY][point.posX].blockValue = EMPTY;
                                _grid[point.posY][point.posX].collisionValue = C_NOTHING;
                                changes += tileToJSON(point.posX, point.posY, EMPTY);
                            }
                        }
                    }
                    // Checks if the player is on a lock, and if so, if all the players stand on a lock
                    else
                    {
                        for (Point p : _lockPosition)
                        {
                            if (newPosX == p.posX && newPosY == p.posY)
                            {
                                std::cout << "OnLock player " << playerID << std::endl;
                                _players[playerID]->setOnLock(true);
                                _finished = true;
                                _foundLock = true;
                                for (Player *p : _players)
                                {
                                    if (!p->isOnLock())
                                    {
                                        std::cout << "But not OnLock for player " << p->getInGameID() << " so not finished game" << std::endl;
                                        _finished = false;
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
            p->setLastCollisionType(_grid[newPosY][newPosX].collisionValue);
        }
        // Checks if the player tries to go through a ghost wall
        else if (p->getRole() == CREATE && _grid[newPosY][newPosX].blockValue == GHOST_WALLS && (posX != newPosX || posY != newPosY))
        {
            _grid[posY][posX].collisionValue = p->getLastCollisionType();
            p->setPos(newPosX, newPosY);
            p->setLastCollisionType(C_BLOCK);
        }
        // Checks if the players tries to go through a ladder or ennemies
        else if (p->getRole() == CLIMB && (posX != newPosX || posY != newPosY))
        {
            if (_grid[newPosY][newPosX].blockValue == LADDER || (((IS_ENEMY_BLOCK(_grid[newPosY][newPosX].blockValue)) && p->hasSecondaryAction())))
            {
                _grid[posY][posX].collisionValue = p->getLastCollisionType();
                p->setPos(newPosX, newPosY);
                p->setLastCollisionType(C_BLOCK);
            }
        }
    }
    p->setLastDirection(direction);

    // Checks if the player no longer stands on a lock.
    // If it is still on a lock, this will have no effect
    // as the boolean is already set
    p->setOnLock(_foundLock);

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

bool Game::getFinished() { return _finished; }

void Game::setFinished(bool finished) { _finished = finished; }

std::string Game::checkPush(std::string dir, int posX, int posY, Player *p)
{
    std::string res = "";
    int pushX = posX, pushY = posY;
    // Retrieves the target position (= tile after the boulder)
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

        // Is the target position valid?
        if (pushX >= 0 && pushX < _width && pushY >= 0 && pushY < _height && (_grid[pushY][pushX].blockValue == PIT || _grid[pushY][pushX].collisionValue == C_NOTHING))
        {
            _grid[p->getPosY()][p->getPosX()].collisionValue = p->getLastCollisionType();
            p->setPos(posX, posY);
            _grid[posY][posX].blockValue = EMPTY;
            _grid[posY][posX].collisionValue = C_WALKABLE;
            p->setLastCollisionType(_grid[posY][posX].collisionValue);

            res += tileToJSON(posX, posY, EMPTY);
            res += ',';

            // Is the boulder on a pit? If yes, we can now walk on it
            if (_grid[pushY][pushX].blockValue == PIT)
            {
                _grid[pushY][pushX].blockValue = MOVABLE_GROUNDED;
                _grid[pushY][pushX].collisionValue = C_NOTHING;
                res += tileToJSON(pushX, pushY, MOVABLE_GROUNDED);
            }
            else
            {
                _grid[pushY][pushX].blockValue = MOVABLE;
                _grid[pushY][pushX].collisionValue = C_BLOCK;
                res += tileToJSON(pushX, pushY, MOVABLE);
            }
        }
    }

    return res;
}

std::string Game::checkJump(std::string dir, int posX, int posY, Player *p)
{
    std::string res = "";
    int jumpX = posX, jumpY = posY;

    // Retrieves the target position (= tile after the pit)
    if (_grid[posY][posX].blockValue == PIT)
    {
        if (dir == "up")
        {
            --jumpY;
        }
        else if (dir == "down")
        {
            ++jumpY;
        }
        else if (dir == "left")
        {
            --jumpX;
        }
        else if (dir == "right")
        {
            ++jumpX;
        }

        // Is the target position valid?
        if (jumpX >= 0 && jumpX < _width && jumpY >= 0 && jumpY < _height && _grid[jumpY][jumpX].collisionValue < C_BLOCK)
        {
            _grid[p->getPosY()][p->getPosX()].collisionValue = p->getLastCollisionType();
            p->setLastCollisionType(_grid[jumpY][jumpX].collisionValue);
            p->setPos(jumpX, jumpY);
        }
    }

    return res;
}

std::string Game::checkPassGhostWall()
{
    // Empty action
    std::string res = "";
    return res;
}

std::string Game::checkCreate(int posX, int posY)
{
    std::string res = "";
    if (_grid[posY][posX].backgroundValue == WATER)
    {
        _grid[posY][posX].blockValue = LILYPAD;
        _grid[posY][posX].collisionValue = C_NOTHING;
        res += tileToJSON(posX, posY, LILYPAD);
    }
    // TO DO: maybe more types of blocks
    return res;
}

bool Game::checkPlayerOnBlock(Block b)
{
    for (Player *p : _players)
    {
        if (p->getPosX() == b.p.posX && p->getPosY() == b.p.posY)
        {
            return true;
        }
    }
    return false;
}

std::string Game::checkActivate(int posX, int posY)
{
    std::string res = "";

    if (_grid[posY][posX].blockValue == BUTTON)
    {
        int i = 0;
        if (!_buttonState)
        {
            for (Block onb : _onBlocks)
            {
                // Is it safe to activate the button?
                if (checkPlayerOnBlock(onb))
                {
                    for (int j = 0; j < i; ++i)
                    {
                        _grid[onb.p.posY][onb.p.posX].blockValue = EMPTY;
                        _grid[onb.p.posY][onb.p.posX].collisionValue = C_NOTHING;
                    }
                    return "";
                }
                if (i)
                    res += ',';
                _grid[onb.p.posY][onb.p.posX].blockValue = onb.value;
                _grid[onb.p.posY][onb.p.posX].collisionValue = C_BLOCK;
                res += tileToJSON(onb.p.posX, onb.p.posY, onb.value);
                ++i;
            }
            for (Block ofb : _offBlocks)
            {
                res += ',';
                _grid[ofb.p.posY][ofb.p.posX].blockValue = EMPTY;
                _grid[ofb.p.posY][ofb.p.posX].collisionValue = C_NOTHING;
                res += tileToJSON(ofb.p.posX, ofb.p.posY, EMPTY);
            }
        }
        else
        {
            for (Block onb : _onBlocks)
            {
                // Is it safe to activate the button?
                if (checkPlayerOnBlock(onb))
                {
                    for (int j = 0; j < i; ++i)
                    {
                        _grid[onb.p.posY][onb.p.posX].blockValue = EMPTY;
                        _grid[onb.p.posY][onb.p.posX].collisionValue = C_NOTHING;
                    }
                    return "";
                }
                if (i)
                    res += ',';
                _grid[onb.p.posY][onb.p.posX].blockValue = EMPTY;
                _grid[onb.p.posY][onb.p.posX].collisionValue = C_NOTHING;
                res += tileToJSON(onb.p.posX, onb.p.posY, EMPTY);
                ++i;
            }
            for (Block ofb : _offBlocks)
            {
                res += ',';
                _grid[ofb.p.posY][ofb.p.posX].blockValue = ofb.value;
                _grid[ofb.p.posY][ofb.p.posX].collisionValue = C_BLOCK;
                res += tileToJSON(ofb.p.posX, ofb.p.posY, ofb.value);
            }
        }
        _buttonState = !_buttonState;
    }

    return res;
}

std::string Game::checkTeleport(Player *p)
{
    std::string res = "";
    //dummy not set up -> set it up on player's position
    Point *dummy = p->getDummy();
    if (dummy == nullptr)
    {
        //Can't create a dummy on a fence
        if (!_buttonState)
        {
            for (Block onb : _onBlocks)
            {
                // Is it safe to activate the button?
                if (checkPlayerOnBlock(onb))
                {
                    return res;
                }
            }
        }
        else
        {
            for (Block ofb : _offBlocks)
            {
                // Is it safe to activate the button?
                if (checkPlayerOnBlock(ofb))
                {
                    return res;
                }
            }
        }
        
        dummy = p->setDummy();
        res += tileToJSON(dummy->posX, dummy->posY, TELEPORT);
        //changes
        _grid[dummy->posY][dummy->posX].collisionValue = C_WALKABLE;
        _grid[dummy->posY][dummy->posX].blockValue = TELEPORT;
        p->setLastCollisionType(C_WALKABLE);
        res += ',';
        res += tileToJSON(dummy->posX, dummy->posY, TELEPORT);
    }
    //dummy set up: tp the player on it
    else if (p->getLastCollisionType() == C_NOTHING)
    {
        _grid[p->getPosY()][p->getPosX()].collisionValue = p->getLastCollisionType();
        p->Teleport();
        res += tileToJSON(dummy->posX, dummy->posY, EMPTY);
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

std::string Game::checkKill(int posX, int posY)
{
    std::string res = "";
    if (IS_ENEMY_BLOCK(_grid[posY][posX].blockValue))
    {
        _grid[posY][posX].blockValue = EMPTY;
        _grid[posY][posX].collisionValue = C_NOTHING;
        res += tileToJSON(posX, posY, EMPTY);
    }
    return res;
}

std::string Game::checkPassLadder()
{
    // Empty action
    std::string res = "";
    return res;
}

std::string Game::checkPassEnemy()
{
    // Empty action
    std::string res = "";
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
        std::string actionString = "";
        switch (p->getRole())
        {
        // Executes the main action of the player. If it is not possible,
        // tries to execute the secondary if it is enabled
        case PUSH:
            actionString = checkPush(dir, posX, posY, p);

            if (actionString == "" && p->hasSecondaryAction())
            {
                actionString = checkJump(dir, posX, posY, p);
            }
            break;
        case CREATE:
            actionString = checkPassGhostWall();
            if (actionString == "" && p->hasSecondaryAction())
            {
                actionString = checkCreate(posX, posY);
            }
            break;
        case ACTIVATE:
            actionString = checkActivate(posX, posY);
            if (actionString == "" && p->hasSecondaryAction())
            {
                actionString = checkTeleport(p);
            }
            break;
        case BREAK:
            actionString = checkBreak(posX, posY);
            if (actionString == "" && p->hasSecondaryAction())
            {
                actionString = checkKill(posX, posY);
            }
            break;
        case CLIMB:
            // These methods don't do anything, but are left in case we add something
            actionString = checkPassLadder();
            if (actionString == "" && p->hasSecondaryAction())
            {
                actionString = checkPassEnemy();
            }
            break;
        }
        res += actionString;
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
            it->setConnected(true);
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
            _grid[j][i].backgroundValue = value;
            _grid[j][i].blockValue = EMPTY;
            _grid[j][i].collisionValue = C_NOTHING;
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

void Game::readObjects(RSJresource layerResource)
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
            _grid[j][i].collisionValue = value - 1;
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
    size_t cpt = 0;
    while (!ss.eof())
    {
        ss >> tmp;
        if (std::stringstream(tmp) >> value && value != 0 && value - 1 <= BREAK)
        {
            if (cpt < _players.size())
            {
                _players[cpt++]->setPos(i, j);
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

        for(int i = 0; i < _height; ++i)
        {
            std::vector<Tile> v;
            for(int j = 0; j < _width; ++j)
            {
                v.push_back(Tile{0, 0, C_NOTHING});
            }
            _grid.push_back(v);
        }

        for (auto it = json_file_resource["layers"].as_array().begin(); it != json_file_resource["layers"].as_array().end(); ++it)
        {
            RSJresource layerResource = it->as<RSJresource>();

            if (layerResource["name"].as<std::string>() == GROUND_LAYER)
            {
                std::cout << "reading ground" << std::endl;
                readBackground(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == COLLISION_LAYER)
            {
                std::cout << "reading collision" << std::endl;
                readCollision(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == KEYS_LAYER)
            {
                std::cout << "reading keys" << std::endl;
                readKey(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == ON_BUTTON_LAYER)
            {
                std::cout << "reading onbuttons" << std::endl;
                readButtonOn(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == OFF_BUTTON_LAYER)
            {
                std::cout << "reading offbuttons" << std::endl;
                readButtonOff(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == PLAYERS_LAYER)
            {
                std::cout << "reading players" << std::endl;
                readPlayersStartPos(layerResource);
            }
            else if (layerResource["name"].as<std::string>() == OBJECTS_LAYER)
            {
                std::cout << "reading objects" << std::endl;
                readObjects(layerResource);
            }
        }
    }
}

std::string Game::getCurrentStateToJSON()
{
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
        }
        mapJSON += ']';
        objectsJSON += ']';
    }
    mapJSON += "],";
    objectsJSON += "],";
    mapJSON += objectsJSON;
    return mapJSON;
}

std::string Game::getMapToJSON()
{
    readMap();
    return getCurrentStateToJSON();
}

std::string Game::getPlayersToJSON()
{
    std::string playersJSON = "\"Players\":[";
    size_t i = 0;
    for (Player *p : _players)
    {
        playersJSON += "{\"xPos\":" + std::to_string(p->getPosX());
        playersJSON += ",\"yPos\":" + std::to_string(p->getPosY());
        playersJSON += ",\"Role\":" + std::to_string(p->getRole());
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

void Game::resetGame()
{
    _buttonState = false, _finished = false, _nbKeys = 0;
    for (Player *p : _players)
    {
        p->setSecondaryAction(false);
    }
    for(int i = 0; i < _height; ++i)
    {
        _grid[i].clear();
    }
    _grid.clear();
}

bool Game::getStarted()
{
    return _started;
}

void Game::setStarted(bool started)
{
    _started = started;
}

Game::~Game()
{
    for (Player *p : _players)
        delete p;
}
