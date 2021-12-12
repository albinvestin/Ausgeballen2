#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include "Vector2.hpp"
#include "Constants.hpp"
#include <cmath>

struct Bullet
{
    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    // Constructor
    Bullet(float aimDirection, Vec2f playerPos, uint8_t playerIndex)
    : position{playerPos},
      velocity{(float)(BULLET_INIT_VEL * cos(aimDirection)),
                (float)(BULLET_INIT_VEL * sin(aimDirection))},
      playerIndex{playerIndex}
    {};
};

struct Player
{
    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    float aimDirection = 0; // in Radians
    uint8_t score = 0;
    // Constructor
    Player(Vec2f startPos, uint8_t playerIndex)
    : position{startPos}, playerIndex{playerIndex}
    {};
};


#endif /* ENTITIES_HPP */