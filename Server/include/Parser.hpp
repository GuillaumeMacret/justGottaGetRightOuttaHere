#ifndef _PARSER_H_
#define _PARSER_H_

#include "Globals.hpp"
#include <string>
#include <vector>

class Parser
{
    private:
        static Parser *_instance;
        
        Parser();

        int requestGamesList(std::string req, class Server *server);
        int requestChangeRole(std::string req, class Server *server);
        int requestChangeMap(std::string req, class Server *server);
        int requestAction(std::string req, class Server *server);
        int requestCreateGame(std::string req, class Server *server);
        int requestJoinGame(std::string req, class Server *server);
        int requestStartGame(std::string req, class Server *server);
        int requestMove(std::string req, class Server *server);
        int requestNextLevel(std::string req, class Server *server);
        int requestLeaveGame(std::string req, class Server *server);

        /* returns 0 if no error, otherwise 1 */
        int getAction(std::string req, class Server *server);

        ~Parser();

    public:
        static Parser getInstance();
};

Parser *Parser::_instance = nullptr;

#endif