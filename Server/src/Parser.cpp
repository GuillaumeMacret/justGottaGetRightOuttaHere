#include "Parser.hpp"
#include "Server.hpp"

Parser::Parser() {}

int Parser::requestGamesList(std::string req, Server *server) {
    //retrieve info
    server->requestGamesList();
    return 0;
}

int Parser::requestChangeRole(std::string req, Server *server) {
    //retrieve info
    server->requestChangeRole();
    return 0;
}

int Parser::requestChangeMap(std::string req, Server *server) {
    //retrieve info
    server->requestChangeMap();
    return 0;
}

int Parser::requestAction(std::string req, Server *server) {
    //retrieve info
    server->requestAction();
    return 0;
}

int Parser::requestCreateGame(std::string req, Server *server) {
    //retrieve info
    server->requestCreateGame();
    return 0;
}

int Parser::requestJoinGame(std::string req, Server *server) {
    //retrieve info
    server->requestJoinGame();
    return 0;
}

int Parser::requestStartGame(std::string req, Server *server) {
    //retrieve info
    server->requestStartGame();
    return 0;
}

int Parser::requestMove(std::string req, Server *server) {
    //retrieve info
    server->requestMove();
    return 0;
}

int Parser::requestNextLevel(std::string req, Server *server) {
    //retrieve info
    server->requestNextLevel();
    return 0;
}

int Parser::requestLeaveGame(std::string req, Server *server) {
    //retrieve info
    server->requestLeaveGame();
    return 0;
}

int Parser::getAction(std::string req, Server *server) {
    std::string s;
    int res = 1;
    if(s == ACTION_GAMES_LIST) {
        res = requestGamesList(req, server);
    } else if(s == ACTION_CHANGE_ROLE) {
        res = requestChangeRole(req, server);
    } else if(s == ACTION_CHANGE_MAP) {
        res = requestChangeMap(req, server);
    } else if(s == ACTION_ACTION) {
        res = requestAction(req, server);
    } else if(s == ACTION_CREATE_GAME) {
        res = requestCreateGame(req, server);
    } else if(s == ACTION_JOIN_GAME) {
        res = requestJoinGame(req, server);
    } else if(s == ACTION_START_GAME) {
        res = requestStartGame(req, server);
    } else if(s == ACTION_MOVE) {
        res = requestMove(req, server);
    } else if(s == ACTION_NEXT_LEVEL) {
        res = requestMove(req, server);
    } else if(s == ACTION_LEAVE_GAME) {
        res = requestLeaveGame(req, server);
    }
    return res;
}

Parser Parser::getInstance() {
    static Parser instance;
    return instance;
}

Parser::~Parser() {}