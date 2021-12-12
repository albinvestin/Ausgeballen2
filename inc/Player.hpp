#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"
 #include <stdint.h>

class Player
{
private:
    Vec2f _Position;
    Vec2f _Velocity;
    float _AimDirection = 0; // in Radians
    int _playerIndex;
    uint8_t _score = 0;
public:
    Player(Vec2f startPos, int playerIndex);
    ~Player();
    Vec2f UpdatePos();
    float updateAimDirection();
    Vec2f GetPos() const;
    float GetAim() const;
    void AddRecoil();
    int GetPlayerIndex() const;
    Vec2f AddVelocity(Vec2f vel);
    Vec2f GetVelocity();
    Vec2f SetVelocity(Vec2f v);
    void AddToPosition(Vec2f v);
    void AddOneToScore();
    uint8_t GetScore();
};


#endif /* PLAYER_HPP */