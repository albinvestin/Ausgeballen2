#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"

class Player
{
private:
    Vec2 mPos, mVel;
public:
    Player();
    ~Player();
    Vec2 update();
};


#endif /* PLAYER_HPP */