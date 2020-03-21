#include "Game.hpp"
#include "Player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "RSJParser.tcc"

Game::Game(int gameID) : _buttonState(false), _nbPlayers(0), _currentLevel(0), _gameID(gameID), _nbKeys(0) {}

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
    res += "{\"xPos\": " + std::to_string(posX);
    res += ",\"yPos\": " + std::to_string(posY);
    res += ",\"value\": " + std::to_string(_grid[posX][posY].blockValue);
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

void Game::readBackground(RSJresource layerResource) {
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1,layerString.length()-2);
    
    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while(!ss.eof()) {
        ss >> tmp;
        if(std::stringstream(tmp) >> value) _grid[j][i] = Tile {value,0,0};
        tmp="";
        ++i;
        if(i % _width == 0) {
            ++j;
            i = 0;
        }
    }
}

void Game::readBlocks(RSJresource layerResource) {
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1,layerString.length()-2);
    
    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while(!ss.eof()) {
        ss >> tmp;
        if(std::stringstream(tmp) >> value && value) _grid[j][i].blockValue = value;
        tmp="";
        ++i;
        if(i % _width == 0) {
            ++j;
            i = 0;
        }
    }
}

void Game::readKey(RSJresource layerResource) {
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1,layerString.length()-2);
    
    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0, lockIndex = 0;
    while(!ss.eof()) {
        ss >> tmp;
        if(std::stringstream(tmp) >> value && value) {
            if(value == KEY) {
                ++_nbKeys;
            } else {
                _lockPosition[lockIndex++] = Point{i,j};
            }
            _grid[j][i].blockValue = value;
        }
        tmp="";
        ++i;
        if(i % _width == 0) {
            ++j;
            i = 0;
        }
    }
}

void Game::readButtonOn(RSJresource layerResource) {
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1,layerString.length()-2);
    
    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while(!ss.eof()) {
        ss >> tmp;
        if(std::stringstream(tmp) >> value && value) {
            if(value != BUTTON)_onBlocks.push_back(Point{i,j});
            _grid[j][i].blockValue = value;
        }
        tmp="";
        ++i;
        if(i % _width == 0) {
            ++j;
            i = 0;
        }
    }
}

void Game::readButtonOff(RSJresource layerResource) {
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1,layerString.length()-2);
    
    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while(!ss.eof()) {
        ss >> tmp;
        if(std::stringstream(tmp) >> value && value) {
            if(value != BUTTON) _offBlocks.push_back(Point{i,j});
            _grid[j][i].blockValue = value;
        }
        tmp="";
        ++i;
        if(i % _width == 0) {
            ++j;
            i = 0;
        }
    }
}

void Game::readCollision(RSJresource layerResource) {
    std::string layerString = layerResource["data"].as<std::string>();
    layerString = layerString.substr(1,layerString.length()-2);
    
    std::stringstream ss;
    ss << layerString;
    std::string tmp;
    int value, i = 0, j = 0;
    while(!ss.eof()) {
        ss >> tmp;
        if(std::stringstream(tmp) >> value) _grid[j][i].collisionValue = value;
        tmp="";
        ++i;
        if(i % _width == 0) {
            ++j;
            i = 0;
        }
    }
}

void Game::readMap() {
    std::ifstream f("maps/" + _selectedMap + ".json");
    if(f.good()) {
        std::string levelString = "";
        for(std::string line; std::getline (f,line);){
            levelString += line;
        }
        RSJresource json_file_resource(levelString);

        _width = json_file_resource["width"].as<int>();
        _height = json_file_resource["height"].as<int>();
        
        _grid = new Tile *[_height];
        for(int i = 0; i < _height; ++i) {
            _grid[i] = new Tile[_width];
        }

        for(auto it = json_file_resource["layers"].as_array().begin();it!=json_file_resource["layers"].as_array().end(); ++it){
            RSJresource layerResource = it->as<RSJresource>();
            
            if(layerResource["name"].as<std::string>() == "Sol"){
                readBackground(layerResource);
            } else if(layerResource["name"].as<std::string>() == "Collision"){
                readCollision(layerResource);
            } else if(layerResource["name"].as<std::string>() == "Cle"){
                readKey(layerResource);
            } else if(layerResource["name"].as<std::string>() == "Interrupteur 1"){
                readButtonOn(layerResource);
            } else if(layerResource["name"].as<std::string>() == "Interrupteur 2"){
                readButtonOff(layerResource);
            } else {
                readBlocks(layerResource);
            }
        }

    }
}

std::string Game::getMapToJSON()
{
    readMap();

    std::string mapJSON = "\"Blocks\":[";
    std::string objectsJSON = "],\"Objects\":[";
    for (int i = 0; i < _height; ++i)
    {
        if(i) mapJSON += ',';
        mapJSON += '[';
        for (int j = 0; j < _width; ++j)
        {   
            if(j) mapJSON += ',';
            mapJSON += std::to_string(_grid[i][j].backgroundValue);
            if(_grid[i][j].blockValue) {
                objectsJSON += "{\"xPos\":" + std::to_string(j);
                objectsJSON += ",\"yPos\":" + std::to_string(i);
                objectsJSON += ",\"value\":" + std::to_string(_grid[i][j].blockValue);
                objectsJSON += '}';
            }
        }
        mapJSON += ']';
    }
    //mapJSON += objectsJSON;
    mapJSON += ']';
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