#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"

class Player
{
private:
    Vec2f _Pos;
    Vec2f _Vel;
    float _AimDirection = 0; // in Radians 
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