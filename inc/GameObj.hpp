#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "Display.hpp"

class GameObj
{
private:
    Display mDisplay{};
public:
    GameObj();
    ~GameObj();
    void start();
    void close();
};

#endif /* GAMEOBJ_HPP */
