#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"

class Player
{
private:
    Vector2 mPos, mVel;
public:
    Player();
    ~Player();
    Vector2 update();
};


#endif /* PLAYER_HPP */