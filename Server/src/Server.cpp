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
            
            // Game * g = getGameFromPlayer(index);
            // g->removePlayer(index);
            // if (g->getPlayers().empty())
            // {
            //     for (auto it = _games.begin(); it != _games.end(); ++it)
            //     {
            //         if ((*it)->getGameID() == g->getGameID())
            //         {
            //             _games.erase(it);
            //         }
            //     }
            // }
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
            _players[cliNum] = new Player(cliNum);
            std::thread t(&Server::runPlayer, this, cliNum);
            t.detach();
            i++;
        }
    }
}

void Server::requestGamesList(int userIndex)
{
    //just send the list to the dest
    std::string answer = "{Action:\"" ACTION_GAMES_LIST "\", Games:[";

    for (Game *g : _games)
    {
        answer += "Game:{id:" + g->getGameID();
        answer += ", nbPlayers:" + g->getPlayers().size();
    }

    answer += "]}\n";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestChangeRole(int userIndex, int roleID)
{
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        std::string answer = "{Action:\"" ACTION_CHANGE_ROLE "\", ";
        answer += "PlayerId:" + _players[userIndex]->getInGameID();
        answer += ", RoleId:" + roleID;
        answer += "}\n";
        broadcastGame(g, answer);
    }
}

void Server::requestChangeMap(int userIndex, std::string mapName)
{
    Game *g = getGameFromPlayer(userIndex);
    std::string answer = "{Action:\"" ACTION_CHANGE_MAP "\", ";
    if (g != nullptr)
    {
        g->changeMap(mapName);
        answer += "Map:\"" + mapName + "\"}\n";
        broadcastGame(g, answer);
    }
    else
    {
        //error sent to the requesting player
    }
}

void Server::requestAction(int userIndex)
{
    //TO DO: change the map state according to the action
    TCPConn.answers[userIndex] = "OK";
}

void Server::requestCreateGame(int userIndex)
{
    Game *g = new Game(_games.size() + 1);
    Player *p = new Player(userIndex, g);
    g->addPlayer(p);
    _games.push_back(g);
    std::string answer = "{Action:\"" ACTION_CREATE_GAME "\"}\n";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestJoinGame(int userIndex, int gameID)
{
    std::string answer;

    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        if (g->getPlayers().size() < 4)
        {
            g->addPlayer(_players[userIndex]);

            answer = "{Action:\"" ACTION_JOINED_GAME ", GameId:" + gameID;
            answer += ", Players:[";
            int i = 0;
            for (Player *p : g->getPlayers())
            {
                if (i)
                    answer += ", ";
                answer += p->getIndex();
                ++i;
            }
            answer += "], PlayerId:" + _players[userIndex]->getInGameID();
            answer += ", Map:\"" + g->getMapName();
            answer += "\"}\n";

            broadcastGame(g, answer);
            return;
        }
        else
        {
            answer = "{Action:\"" ACTION_CANT_JOIN_GAME ", GameId:" + gameID;
            answer += "MoreInfo:\"" ERROR_GAME_FULL "\"";
        }
    }
    else
    {
        answer = "{Action:\"" ACTION_CANT_JOIN_GAME ", GameId:" + gameID;
        answer += "MoreInfo:\"" ERROR_GAME_DOES_NOT_EXIST "\"";
    }
    answer += "}\n";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestStartGame(int userIndex)
{
    //TO DO: init players position and the map using Game::_selectedMap
    std::string answer;

    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        std::vector<Player *> players = g->getPlayers();
        bool available = true;
        for (size_t i = 0; i < players.size() - 1; ++i)
        {
            for (size_t j = i + 1; j < players.size(); ++j)
            {
                if (players[i]->getRole() == players[j]->getRole())
                {
                    available = false;
                    i = j = players.size();
                }
            }
        }
        if (available)
        {
            answer = "{Action:\"" ACTION_LOAD_LEVEL "\", Level:";
            answer += g->getMapToJSON();
            answer += g->getPlayersToJSON();
            answer += "\n";
        }
        else
        {
        }
    }
    broadcastGame(g, answer);
}

void Server::requestMove(int userIndex, std::string moveDir)
{
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        g->movePlayer(_players[userIndex]->getInGameID(), moveDir);
    }

    std::string answer;
    answer = "{Action:\"" ACTION_MOVE "\", PosX:" + _players[userIndex]->getPosX();
    answer += ", PosY:" + _players[userIndex]->getPosY();
    answer += ", Player:" + _players[userIndex]->getInGameID();
    answer += "}\n";

    broadcastGame(g, answer);
}

void Server::requestNextLevel(int userIndex)
{
    //TO DO: init players position and the map
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        g->increaseLevel();
    }

    std::string answer;
    answer = "{Action:\"" ACTION_LOAD_LEVEL "\", Level:";
    answer += g->getMapToJSON();
    answer += g->getPlayersToJSON();
    answer += "\n";
    broadcastGame(g, answer);
}

void Server::requestLeaveGame(int userIndex)
{
    //delete the player, and the game if empty
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        g->removePlayer(userIndex);
        if (g->getPlayers().empty())
        {
            for (auto it = _games.begin(); it != _games.end(); ++it)
            {
                if ((*it)->getGameID() == g->getGameID())
                {
                    _games.erase(it);
                }
            }
        }
    std::string answer;
    answer = "{Action:\"" ACTION_LEAVE_GAME "\", Player:" + _players[userIndex]->getInGameID();
    answer += "}\n";
    broadcastGame(g, answer);
    }
}

Game *Server::getGameFromPlayer(int userIndex)
{
    return _players[userIndex]->getGame();
}

void Server::broadcastGame(Game *game, std::string msg)
{
    for (Player *p : game->getPlayers())
    {
        if (TCPConn.server_send(p->getIndex(), msg) == ERR)
        {
            // Game *g = p->getGame();
            // g->removePlayer(p->getIndex());
            // if (g->getPlayers().empty())
            // {
            //     for (auto it = _games.begin(); it != _games.end(); ++it)
            //     {
            //         if ((*it)->getGameID() == g->getGameID())
            //         {
            //             _games.erase(it);
            //         }
            //     }
            // }
        }
    }
}