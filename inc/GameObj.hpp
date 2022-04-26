#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "Display.hpp"

class GameObj
{
private:
    Display _display{};
public:
    GameObj();
    ~GameObj();
    void start();
    void close();
};

#endif /* GAMEOBJ_HPP */
