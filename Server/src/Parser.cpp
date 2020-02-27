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

int Parser::requestGamesList(std::string &req, Server &server)
{
    server.requestGamesList();
    return 0;
}

int Parser::requestChangeRole(std::string &req, Server &server)
{
    int roleID;
    std::string res = readArg(req);
    if (res != "")
    {
        roleID = atoi(res.c_str());
        server.requestChangeRole(roleID);
        return 0;
    }
    return 1;
}

int Parser::requestChangeMap(std::string &req, Server &server)
{
    std::string mapName = readArg(req);
    if (mapName != "")
    {
        server.requestChangeMap(mapName);
        return 0;
    }
    return 1;
}

int Parser::requestAction(std::string &req, Server &server)
{
    server.requestAction();
    return 0;
}

int Parser::requestCreateGame(std::string &req, Server &server)
{
    server.requestCreateGame();
    return 0;
}

int Parser::requestJoinGame(std::string &req, Server &server)
{
    int gameID;
    std::string res = readArg(req);
    if (res != "")
    {
        gameID = atoi(res.c_str());
        server.requestJoinGame(gameID);
        return 0;
    }
    return 1;
}

int Parser::requestStartGame(std::string &req, Server &server)
{
    server.requestStartGame();
    return 0;
}

int Parser::requestMove(std::string &req, Server &server)
{
    std::string moveDir = readArg(req);
    if (moveDir != "")
    {
        server.requestMove(moveDir);
        return 0;
    }
    return 1;
}

int Parser::requestNextLevel(std::string &req, Server &server)
{
    server.requestNextLevel();
    return 0;
}

int Parser::requestLeaveGame(std::string &req, Server &server)
{
    server.requestLeaveGame();
    return 0;
}

int Parser::getAction(std::string &req, Server &server)
{
    std::string s = readArg(req);

    int res = 1;
    if (s != "")
    {
        if (s == ACTION_GAMES_LIST)
        {
            res = requestGamesList(req, server);
        }
        else if (s == ACTION_CHANGE_ROLE)
        {
            res = requestChangeRole(req, server);
        }
        else if (s == ACTION_CHANGE_MAP)
        {
            res = requestChangeMap(req, server);
        }
        else if (s == ACTION_ACTION)
        {
            res = requestAction(req, server);
        }
        else if (s == ACTION_CREATE_GAME)
        {
            res = requestCreateGame(req, server);
        }
        else if (s == ACTION_JOIN_GAME)
        {
            res = requestJoinGame(req, server);
        }
        else if (s == ACTION_START_GAME)
        {
            res = requestStartGame(req, server);
        }
        else if (s == ACTION_MOVE)
        {
            res = requestMove(req, server);
        }
        else if (s == ACTION_NEXT_LEVEL)
        {
            res = requestMove(req, server);
        }
        else if (s == ACTION_LEAVE_GAME)
        {
            res = requestLeaveGame(req, server);
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