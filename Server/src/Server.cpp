#include "TCPConnection.hpp"
#include "Server.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "Parser.hpp"
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

        std::string answer;
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

void Server::requestGamesList(int userIndex) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestChangeRole(int userIndex, int roleID) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestChangeMap(int userIndex, std::string mapName) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestAction(int userIndex) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestCreateGame(int userIndex) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestJoinGame(int userIndex, int gameID) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestStartGame(int userIndex) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestMove(int userIndex, std::string moveDir) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestNextLevel(int userIndex) { TCPConn.answers[userIndex] = "OK"; }
void Server::requestLeaveGame(int userIndex) { TCPConn.answers[userIndex] = "OK"; }