#include "Server.hpp"
#include "Globals.hpp"
#include "Game.hpp"
#include <stdio.h>

Server::Server() {}

Server::~Server() {
    for(Game *g : _games) delete g;
}