#include "Parser.hpp"
#include "Server.hpp"
#include <iostream>

Parser::Parser() {}

std::string Parser::readArg(std::string &req)
{
    std::string s;
    size_t pos = req.find(':');
    if (pos != std::string::npos)
    {
        req = req.substr(pos + 2);
        pos = 0;
        for (char c : req)
        {
            if (c == '"')
            {
                req = req.substr(pos + 1);
                break;
            }
            s += c;
            ++pos;
        }
    }
    return s;
}

int Parser::readInt(std::string &req)
{
    std::string s;
    size_t pos = req.find(':');
    if (pos != std::string::npos)
    {
        req = req.substr(pos + 1);
        pos = 0;
        for (char c : req)
        {
            if (!isdigit(c))
            {
                req = req.substr(pos + 1);
                break;
            }
            s += c;
            ++pos;
        }
    }
    return std::stoi(s);
}

int Parser::requestGamesList(std::string &req, Server &server, int userIndex)
{
    server.requestGamesList(userIndex);
    return 0;
}

int Parser::requestChangeRole(std::string &req, Server &server, int userIndex)
{
    int roleID;
    try
    {
        roleID = readInt(req);
        server.requestChangeRole(userIndex, roleID);
    }
    catch (std::exception const &e)
    {
        return 1;
    }

    return 0;
}

int Parser::requestChangeMap(std::string &req, Server &server, int userIndex)
{
    std::string mapName = readArg(req);
    if (mapName != "")
    {
        server.requestChangeMap(userIndex, mapName);
        return 0;
    }
    return 1;
}

int Parser::requestAction(std::string &req, Server &server, int userIndex)
{
    server.requestAction(userIndex);
    return 0;
}

int Parser::requestCreateGame(std::string &req, Server &server, int userIndex)
{
    server.requestCreateGame(userIndex);
    return 0;
}

int Parser::requestJoinGame(std::string &req, Server &server, int userIndex)
{
    int gameID;
    try
    {
        gameID = readInt(req);
        server.requestJoinGame(userIndex, gameID);
    }
    catch (std::exception const &e)
    {
        return 1;
    }

    return 0;
}

int Parser::requestStartGame(std::string &req, Server &server, int userIndex)
{
    server.requestStartGame(userIndex);
    return 0;
}

int Parser::requestMove(std::string &req, Server &server, int userIndex)
{
    std::string moveDir = readArg(req);
    if (moveDir != "")
    {
        server.requestMove(userIndex, moveDir);
        return 0;
    }
    return 1;
}

int Parser::requestNextLevel(std::string &req, Server &server, int userIndex)
{
    server.requestNextLevel(userIndex);
    return 0;
}

int Parser::requestLeaveGame(std::string &req, Server &server, int userIndex)
{
    server.requestLeaveGame(userIndex);
    return 0;
}

int Parser::requestReturnToLobby(std::string &req, class Server &server, int userIndex)
{
    server.requestReturnToLobby(userIndex);
    return 0;
}

int Parser::requestSendPing(std::string &req, class Server &server, int userIndex)
{
    int posX, posY;
    try
    {
        posX = readInt(req);
        posY = readInt(req);
        server.requestSendPing(userIndex, posX, posY);
    }
    catch (std::exception const &e)
    {
        return 1;
    }
    return 0;
}

int Parser::requestNoticeReady(std::string &req, Server &server, int userIndex)
{
    server.requestNoticeReady(userIndex);
    return 0;
}

int Parser::getAction(std::string &req, Server &server, int userIndex)
{
    std::string s = readArg(req);

    int res = 1;
    if (s != "")
    {
        if (s == ACTION_MOVE)
        {
            res = requestMove(req, server, userIndex);
        }
        else if (s == ACTION_ACTION)
        {
            res = requestAction(req, server, userIndex);
        }
        else if (s == ACTION_SEND_PING)
        {
            res = requestSendPing(req, server, userIndex);
        }
        else if (s == ACTION_GAMES_LIST)
        {
            res = requestGamesList(req, server, userIndex);
        }
        else if (s == ACTION_NOTICE_READY)
        {
            res = requestNoticeReady(req, server, userIndex);
        }
        else if (s == ACTION_CHANGE_ROLE)
        {
            res = requestChangeRole(req, server, userIndex);
        }
        else if (s == ACTION_CHANGE_MAP)
        {
            res = requestChangeMap(req, server, userIndex);
        }
        else if (s == ACTION_CREATE_GAME)
        {
            res = requestCreateGame(req, server, userIndex);
        }
        else if (s == ACTION_JOIN_GAME)
        {
            res = requestJoinGame(req, server, userIndex);
        }
        else if (s == ACTION_START_GAME)
        {
            res = requestStartGame(req, server, userIndex);
        }
        else if (s == ACTION_NEXT_LEVEL)
        {
            res = requestNextLevel(req, server, userIndex);
        }
        else if (s == ACTION_LEAVE_GAME)
        {
            res = requestLeaveGame(req, server, userIndex);
        }
        else if (s == ACTION_RETURN_LOBBY)
        {
            res = requestReturnToLobby(req, server, userIndex);
        }
    }
    return res;
}

Parser Parser::getInstance()
{
    static Parser instance;
    return instance;
}

Parser::~Parser() {}