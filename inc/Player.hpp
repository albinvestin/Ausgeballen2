#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"

class Player
{
private:
    Vec2 mPos, mVel;
    float mAimDirection = 0; // in Radians 
public:
    Player();
    ~Player();
    Vec2 updatePos();
    float updateAimDirection();
    Vec2 GetPos();
    float GetAim();
};


#endif /* PLAYER_HPP */