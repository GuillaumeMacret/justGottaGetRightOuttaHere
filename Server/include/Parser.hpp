#ifndef _PARSER_H_
#define _PARSER_H_

#include "Globals.hpp"
#include <string>
#include <vector>

class Parser
{
private:
    Parser();

    /* Reads and returns the value corresonding of the current field in the JSON string */
    std::string readArg(std::string &req);

    /* Reads an integer in a JSON string, and returns it */
    int readInt(std::string &req);

    /** Reads specific action requests from the clients
     *  Returns 0 if no error, 1 otherwise */
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
    int requestReturnToLobby(std::string &req, class Server &server, int userIndex);

public:
    static Parser getInstance();

    /** Retrieves the action requested by the player, and call the associated function
     * Returns 0 if no error, 1 otherwise*/
    int getAction(std::string &req, class Server &server, int userIndex);

    ~Parser();
};

#endif