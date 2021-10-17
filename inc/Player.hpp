#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"

class Player
{
private:
    Vec2f _Position;
    Vec2f _Velocity;
    float _AimDirection = 0; // in Radians
    int _playerIndex;
public:
    Player(Vec2f startPos, int playerIndex);
    ~Player();
    Vec2f UpdatePos();
    float updateAimDirection();
    Vec2f GetPos();
    float GetAim();
    void AddRecoil();
    int GetPlayerIndex();
    Vec2f AddVelocity(Vec2f vel);
};


#endif /* PLAYER_HPP */