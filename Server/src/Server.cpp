#include "TCPConnection.hpp"
#include "Server.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "Parser.hpp"
#include "Player.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <dirent.h>
#include <string.h>
#include <algorithm>
#include <vector>

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
    std::string req;
    for (;;)
    {
        req = TCPConn.server_receive(index);
        std::cout << "receive -" << req << "- from user : " << index << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(3));

        if (req == "" || Parser::getInstance().getAction(req, *this, index) != 0)
        {
            TCPConn.answers[index] = "Error";
        }

        std::cout << "Sending this to player " << _players[index]->getInGameID() << " : " << TCPConn.answers[index] << std::endl;

        if (!TCPConn.answers[index].empty() && TCPConn.server_send(index) == ERR)
        {
            removePlayerFromGame(index);
            return;
        }
        req.clear();
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
    std::string answer = "{\"Action\":\"" ACTION_GAMES_LIST "\", \"Games\":[";
    if (!_games.empty())
    {
        size_t i = 0;
        for (; i < _games.size() - 1; ++i)
        {
            answer += "{\"id\":" + std::to_string(_games[i]->getGameID());
            answer += ", \"nbPlayers\":" + std::to_string(_games[i]->getNbConnectedPlayers());
            answer += "},";
        }
        answer += "{\"id\":" + std::to_string(_games[i]->getGameID());
        answer += ", \"nbPlayers\":" + std::to_string(_games[i]->getNbConnectedPlayers());
        answer += '}';
    }
    answer += "]};\n";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestChangeRole(int userIndex, int roleID)
{
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        _players[userIndex]->setRole(roleID);
        std::string answer = "{\"Action\":\"" ACTION_CHANGE_ROLE "\", ";
        answer += "\"PlayerId\":" + std::to_string(_players[userIndex]->getInGameID());
        answer += ", \"RoleId\":" + std::to_string(roleID);
        answer += "};\n";
        broadcastGame(g, answer);
    }
}

void Server::requestChangeMap(int userIndex, std::string mapName)
{
    Game *g = getGameFromPlayer(userIndex);
    std::string answer = "{\"Action\":\"" ACTION_CHANGED_MAP "\", ";
    if (g != nullptr)
    {
        g->changeMap(mapName);
        answer += "\"Map\":\"" + mapName + "\"};\n";
        broadcastGame(g, answer);
    }
    else
    {
        //error sent to the requesting player
    }
}

void Server::requestAction(int userIndex)
{
    Game *g = getGameFromPlayer(userIndex);
    //TO DO: change the map state according to the action

    if (g != nullptr)
    {
        std::string answer, changes = g->doActionPlayer(_players[userIndex]->getInGameID());
        answer = "{\"Action\":\"" ACTION_ACTION "\", \"PosX\":" + std::to_string(_players[userIndex]->getPosX());
        answer += ", \"PosY\":" + std::to_string(_players[userIndex]->getPosY());
        answer += ", \"Player\":" + std::to_string(_players[userIndex]->getInGameID());
        answer += ", \"Changes\":" + changes;
        answer += "};\n";
        broadcastGame(g, answer);
    }
}

void Server::requestCreateGame(int userIndex)
{
    Game *g = new Game(_games.size());
    _players[userIndex]->setGame(g);
    g->addPlayer(_players[userIndex]);
    _games.push_back(g);

    std::string answer = "{\"Action\":\"" ACTION_CREATED_GAME "\",";
    answer += "\"GameId\":" + std::to_string(g->getGameID()) + ',';
    answer += "\"MapList\":[";

    DIR *mapDir;
    struct dirent *map;
    std::string mapName;
    std::vector<std::string> mapNames;
    if ((mapDir = opendir("./maps")))
    {
        while ((map = readdir(mapDir)))
        {
            if (strcmp(map->d_name, ".") != 0 && strcmp(map->d_name, "..") != 0)
            {
                mapNames.push_back(map->d_name);
            }
        }
        std::sort(mapNames.begin(), mapNames.end());
        int i = 0;
        for(std::string s : mapNames)
        {
            if (i)
                answer += ',';
            mapName = s;
            mapName = mapName.substr(0, s.find(".json"));
            answer += "\"" + mapName + "\"";
            ++i;
        }
        closedir(mapDir);
    }

    answer += "]};\n";

    TCPConn.answers[userIndex] = answer;
}

void Server::requestJoinGame(int userIndex, int gameID)
{
    std::string answer;
    Game *g = nullptr;
    if (gameID < MAX_CONNECTION_TCP && gameID >= 0)
    {
        g = _games[gameID];
    }
    if (g != nullptr)
    {
        if (g->addPlayer(_players[userIndex]))
        {
            if (!g->getStarted())
            {

                answer = "{\"Action\":\"" ACTION_JOINED_GAME "\", \"GameId\":" + std::to_string(g->getGameID());
                answer += ", \"PlayersRoles\":[";
                int i = 0;
                for (Player *p : g->getPlayers())
                {
                    if (i)
                        answer += ", ";
                    answer += std::to_string(p->getRole());
                    ++i;
                }
                answer += "], \"PlayerId\":" + std::to_string(_players[userIndex]->getInGameID());
                answer += ", \"Map\":\"" + g->getMapName();
                //answer += ", \"Lobby\":\"" + g->isInLobby();
                answer += "\"};\n";

                broadcastGame(g, answer);
                return;
            }
            else
            {
                answer = "{\"Action\":\"" ACTION_LOAD_LEVEL "\", ";
                answer += g->getCurrentStateToJSON();
                answer += g->getPlayersToJSON();
                answer += "};\n";
                TCPConn.answers[userIndex] = answer;
                return;
            }
        }
        else
        {
            answer = "{\"Action\":\"" ACTION_CANT_JOIN_GAME "\", \"GameId\":" + std::to_string(gameID);
            answer += ", \"MoreInfo\":\"" ERROR_GAME_FULL "\"";
        }
    }
    else
    {
        answer = "{\"Action\":\"" ACTION_CANT_JOIN_GAME "\", \"GameId\":" + std::to_string(gameID);
        answer += ", \"MoreInfo\":\"" ERROR_GAME_DOES_NOT_EXIST "\"";
    }
    answer += "};\n";
    TCPConn.answers[userIndex] = answer;
}

void Server::requestStartGame(int userIndex)
{
    std::string answer;

    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr && g->getNbConnectedPlayers() == 4)
    {
        g->resetGame();
        std::vector<Player *> players = g->getPlayers();
        /*bool available = true;
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
        }*/
        std::cout << "Loading map called -" << g->getMapName() << "-" << std::endl;
        if (/*available && */g->getMapName() != "")
        {
            g->setStarted(true);
            //g->setInLobby(false);
            answer = "{\"Action\":\"" ACTION_LOAD_LEVEL "\", ";
            answer += g->getMapToJSON();
            answer += g->getPlayersToJSON();
            answer += "};\n";
        }
        else
        {
            answer = "{\"Action\":\"" ACTION_CANT_START_GAME "\", \"GameId\":" + std::to_string(g->getGameID());
            answer += ", \"MoreInfo\":\"" ERROR_DUPLICATE_ROLE "\"};\n";
        }
    }
    else
    {
        answer = "{\"Action\":\"" ACTION_CANT_START_GAME "\", \"GameId\":" + std::to_string(g->getGameID());
        answer += ", \"MoreInfo\":\"" ERROR_NOT_ENOUGH_PLAYERS "\"};\n";
    }
    broadcastGame(g, answer);
}

void Server::requestMove(int userIndex, std::string moveDir)
{
    Game *g = getGameFromPlayer(userIndex);
    std::string changes = "";
    if (g != nullptr)
    {
        changes = g->movePlayer(_players[userIndex]->getInGameID(), moveDir);
    }

    std::string answer;
    answer = "{\"Action\":\"" ACTION_MOVE "\", \"PosX\":" + std::to_string(_players[userIndex]->getPosX());
    answer += ", \"PosY\":" + std::to_string(_players[userIndex]->getPosY());
    answer += ", \"Player\":" + std::to_string(_players[userIndex]->getInGameID());
    answer += ", \"Changes\":[" + changes;
    answer += "]};\n";

    broadcastGame(g, answer);

    if (g->getFinished())
    {
        std::cout << "Game is finished" << std::endl;
        answer = "{\"Action\":\"win\"};\n";
        broadcastGame(g, answer);
        g->setFinished(false);
    }
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
    answer = "{\"Action\":\"" ACTION_LOAD_LEVEL "\", \"Level\":";
    answer += g->getMapToJSON();
    answer += g->getPlayersToJSON();
    answer += ";\n";
    broadcastGame(g, answer);
}

void Server::requestLeaveGame(int userIndex)
{
    //delete the player, and the game if empty
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        removePlayerFromGame(userIndex);
        std::string answer;
        answer = "{\"Action\":\"" ACTION_LEAVE_GAME "\", \"Player\":" + std::to_string(_players[userIndex]->getInGameID());
        answer += "};\n";

        //Need broadcast + answer because we are no longer in the game
        broadcastGame(g, answer);
        TCPConn.answers[userIndex] = answer;
    }
}

void Server::requestReturnToLobby(int userIndex)
{
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        g->resetGame();
        g->setStarted(false);
        std::string answer;
        answer = "{\"Action\":\"" ACTION_RETURNED_LOBBY "\"};\n";
        broadcastGame(g, answer);
    }
}

void Server::requestSendPing(int userIndex, int posX, int posY)
{
    Game *g = getGameFromPlayer(userIndex);
    if (g != nullptr)
    {
        std::string answer;
        answer = "{\"Action\":\"" ACTION_SEND_PING "\", \"PosX\":" + std::to_string(posX);
        answer += ", \"PosY\":" + std::to_string(posY);
        answer += "};\n";
        broadcastGame(g, answer);
    }
}

Game *Server::getGameFromPlayer(int userIndex)
{
    return _players[userIndex]->getGame();
}

void Server::broadcastGame(Game *game, std::string msg)
{
    std::cout << "Sending this to all player in game : " << msg << std::endl;
    for (Player *p : game->getPlayers())
    {
        if (p->isConnected() && TCPConn.server_send(p->getIndex(), msg) == ERR)
        {
            removePlayerFromGame(p->getIndex());
        }
    }
}

void Server::removePlayerFromGame(int index)
{
    Game *g = getGameFromPlayer(index);
    if (g)
    {
        g->disconnectPlayer(index);
        if (g->isPlayerListEmpty())
        {
            for (auto it = _games.begin(); it != _games.end(); ++it)
            {
                if ((*it)->getGameID() == g->getGameID())
                {
                    _games.erase(it);
                    return;
                }
            }
        }
    }
}
