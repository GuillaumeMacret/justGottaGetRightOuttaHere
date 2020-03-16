#ifndef _PARSER_H_
#define _PARSER_H_

#include "Globals.hpp"
#include <string>
#include <vector>

class Parser
{
private:
    Parser();
    std::string readArg(std::string &req);

    /* returns 0 if no error, otherwise 1 */
    int requestGamesList(std::string &req, class Server &server, int userIndex);
    int requestChangeRole(std::string &req, class Server &server, int userIndex);
    int requestChangeMap(std::string &req, class Server &server, int userIndex);
    int requestAction(std::string &req, class Server &server, int userIndex);
    int requestCreateGame(std::string &req, class Server &server, int userIndex);
    int requestJoinGame(std::string &req, class Server &server, int userIndex);
    int requestStartGame(std::string &req, class Server &server, int userIndex);
    int requestMove(std::string &req, class Server &server, int userIndex);
    int requestNextLevel(std::string &req, class Server &server, int userIndex);
    int requestLeaveGame(std::string &req, class Server &server, int userIndex);

public:
    static Parser getInstance();

    /* returns 0 if no error, otherwise 1 */
    int getAction(std::string &req, class Server &server, int userIndex);

    ~Parser();
};

#endif