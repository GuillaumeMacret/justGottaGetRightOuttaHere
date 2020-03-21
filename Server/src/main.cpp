#include <iostream>
#include "Server.hpp"

//#include "Game.hpp"

using namespace std;

int main()
{
    Server s;
    s.run();
    /*Game *g = new Game(0);
    g->changeMap("map");
    g->getMapToJSON();
    delete g;*/
    return 0;
}