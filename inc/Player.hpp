#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"

class Player
{
private:
    Vec2f mPos;
    Vec2f mVel;
    float mAimDirection = 0; // in Radians 
    float mRecoilSize = 10; // TODO make define
public:
    Player();
    ~Player();
    Vec2f updatePos();
    float updateAimDirection();
    Vec2f GetPos();
    float GetAim();
    void AddRecoil();
};


#endif /* PLAYER_HPP */