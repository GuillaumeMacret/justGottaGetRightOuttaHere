#include "TCPConnection.hpp"
#include "Server.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "Parser.hpp"
#include "Player.hpp"
#include <iostream>
#include <thread>

Server::Server()
{
    TCPConn.server_bind(TCP_PORT);
}

Server::~Server()
{
    for (Game *g : _games)
        delete g;
}

void Server::runPlayer(int index)
{
    for (;;)
    {
        std::string req = TCPConn.server_receive(index);
        std::cout << req << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(3));

        if (Parser::getInstance().getAction(req, *this, index) != 0)
        {
            TCPConn.answers[index] = "Error";
        }

        if (TCPConn.server_send(index) == ERR)
        {
            //if player in game remove him

            return;
        }
    }
}

void Server::run()
{
    TCPConn.server_listen();
    int cliNum, i = 0;
    for (;;)
    {
        if ((cliNum = TCPConn.server_accept()) != ERR)
        {
            printf("tcp connection from %d port : %d\n",
                   TCPConn.cli[cliNum].sin_addr.s_addr,
                   TCPConn.cli[cliNum].sin_port);
            std::thread t(&Server::runPlayer, this, cliNum);
            t.detach();
            i++;
        }
    }
}

void Server::requestGamesList(int userIndex) {
    //just send the list to the dest
    std::string answer = "{Action:\"" ACTION_GAMES_LIST "\", Games:[";

    for(Game *g : _games) {
        answer += "Game:{id:" + g->getGameID();
        answer += ", nbPlayers:" + g->getPlayers().size();
    }

    answer += "]}";
    TCPConn.answers[userIndex] = answer.c_str();
}

void Server::requestChangeRole(int userIndex, int roleID) {
    Game *g = getGameFromPlayer(userIndex);
    if(g != nullptr) {
        std::string answer = "{Action:\"" ACTION_CHANGE_ROLE "\", ";
        bool available = true;
        for(Player *p : g->getPlayers()) {
            if(p->getRole() == roleID) {
                available = false;
                break;
            }
        }
        if(available) {
            answer += "PlayerId:" + userIndex;
            answer += ", RoleId:" + roleID;
            answer += "}";

            for(Player *p : g->getPlayers()) {
                TCPConn.answers[p->getIndex()] = answer;
            }
        } else {
            //TO DO: error role not available
            answer += "}";
            TCPConn.answers[userIndex] = answer;
        }
    }
    //TO DO: error change role => game doesn't exist
}

void Server::requestChangeMap(int userIndex, std::string mapName) {
    Game *g = getGameFromPlayer(userIndex);
    std::string answer = "{Action:\"" ACTION_CHANGE_MAP "\", ";
    if(g != nullptr) {
        g->changeMap(mapName);
        answer += "Map:\"" + mapName + "\"}";
        for(Player *p : g->getPlayers()) {
            TCPConn.answers[p->getIndex()] = answer;
        }
    } else {
        //error sent to the requesting player
    }
}

void Server::requestAction(int userIndex) {
    //TO DO: change the map state according to the action

    TCPConn.answers[userIndex] = "OK";
}

void Server::requestCreateGame(int userIndex) {
    //TO DO: create a new game and a new player, add the player to the game
    Game *g = new Game(_games.size()+1);
    Player *p = new Player(userIndex);
    g->addPlayer(p);
    _games.push_back(g);
    std::string answer = "{Action:\"" ACTION_CREATE_GAME "\"}";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestJoinGame(int userIndex, int gameID) {
    std::string answer;

    Game *g = getGameFromPlayer(userIndex);
    if(g != nullptr) {
        if(g->getPlayers().size() < 4) {
            Player *newp = new Player(userIndex);
            g->addPlayer(newp);

            answer = "{Action:\"" ACTION_JOINED_GAME ", GameId:" + gameID;
            answer += ", Players:[";
            int i = 0;
            for(Player *p : g->getPlayers()) {
                if(i) answer += ", ";
            answer += p->getIndex();
                ++i;
            }
            answer += "], PlayerId:" + userIndex;
            answer += ", Map:\"" + g->getMapName();
            answer += "\"}";
            
            for(Player *p : g->getPlayers()) {
                TCPConn.answers[p->getIndex()] = answer;
            }
            return;
        } else {
            answer = "{Action:\"" ACTION_CANT_JOIN_GAME ", GameId:" + gameID;
            answer += "MoreInfo:\"" ERROR_GAME_FULL "\"";
        }
    } else {
        answer = "{Action:\"" ACTION_CANT_JOIN_GAME ", GameId:" + gameID;
        answer += "MoreInfo:\"" ERROR_GAME_DOES_NOT_EXIST "\"";
    }
    answer += "}";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestStartGame(int userIndex) {
    //TO DO: init players position and the map using Game::_selectedMap

    TCPConn.answers[userIndex] = "OK";
}

void Server::requestMove(int userIndex, std::string moveDir) {
    //TO DO: check if the player can move
    
    TCPConn.answers[userIndex] = "OK";
}

void Server::requestNextLevel(int userIndex) {
    //TO DO: init players position and the map
    Game *g = getGameFromPlayer(userIndex);
    if(g != nullptr) {
        g->increaseLevel();
    }
    
    TCPConn.answers[userIndex] = "OK";
}

void Server::requestLeaveGame(int userIndex) {
    //delete the player, and the game if empty
    Game *g = getGameFromPlayer(userIndex);
    if(g != nullptr) {
        g->removePlayer(userIndex);
        if(g->getPlayers().empty()) {
            for(auto it = _games.begin(); it != _games.end(); ++it) {
                if((*it)->getGameID() == g->getGameID()) {
                    _games.erase(it);
                }
            }
        }
    }

    TCPConn.answers[userIndex] = "OK";
}

Game *Server::getGameFromPlayer(int userIndex) {
    for(Game *g : _games) {
        for(Player *p : g->getPlayers()) {
            if(p->getIndex() == userIndex) {
                return g;
            }
        }
    }
    return nullptr;
}