#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "Display.hpp"

class GameObj
{
private:
    Display _Display{};
public:
    GameObj();
    ~GameObj();
    void start();
    void close();
};

#endif /* GAMEOBJ_HPP */
