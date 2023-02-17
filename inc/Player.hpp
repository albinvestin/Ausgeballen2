#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vector2.hpp"
 #include <stdint.h>

class Player
{
private:
    Vec2f _position;
    Vec2f _Velocity;
    float _AimDirection = 0; // in Radians
    int _playerIndex;
    uint8_t _score = 0;
public:
    Player(const Vec2f &startPos, int playerIndex);
    ~Player();
    // Get methods
    Vec2f GetPos() const;
    float GetAim() const;
    int GetPlayerIndex() const;
    Vec2f GetVelocity() const;
    uint8_t GetScore() const;
    // Update methods
    Vec2f UpdatePos();
    float updateAimDirection();
    void AddRecoil();
    Vec2f AddVelocity(const Vec2f &vel);
    Vec2f SetVelocity(const Vec2f &v);
    void AddToPosition(const Vec2f &v);
    void AddOneToScore();
};


#endif /* PLAYER_HPP */