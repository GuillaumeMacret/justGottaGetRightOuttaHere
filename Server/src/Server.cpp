#include "Server.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include "TCPConnection.hpp"
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

void Server::runPlayer(int fd)
{
    for (;;)
    {
        std::string req = TCPConn.server_receive(fd);
        std::cout << req << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(3));
        if (Parser::getInstance().getAction(req, *this))
        {
            std::cerr << "Error" << std::endl;
        }
        else
        {
            std::string o = "OK";
            TCPConn.server_send(fd, o.c_str());
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
            std::thread t(&Server::runPlayer, this, TCPConn.new_fd[cliNum]);
            t.detach();
            i++;
        }
    }
}

void Server::requestGamesList() {}
void Server::requestChangeRole(int roleID) {}
void Server::requestChangeMap(std::string mapName) {}
void Server::requestAction() {}
void Server::requestCreateGame() {}
void Server::requestJoinGame(int gameID) {}
void Server::requestStartGame() {}
void Server::requestMove(std::string moveDir) {}
void Server::requestNextLevel() {}
void Server::requestLeaveGame() {}