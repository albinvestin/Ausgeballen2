#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include "Vector2.hpp"
#include "Constants.hpp"
#include <vector>
#include <cmath>

struct Bullet
{
    // Constructor
    Bullet() {}
    // Bullet(const Bullet &other) = default; // Copy operator
    Bullet(float aimDirection, Vec2f playerPos, uint8_t playerIndex, uint16_t id)
    : position{playerPos},
      velocity{(float)(BULLET_INIT_VEL * cos(aimDirection)),
               (float)(BULLET_INIT_VEL * sin(aimDirection))},
      playerIndex{playerIndex},
      id{id}
    {}

    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    uint16_t id;

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(position, velocity, playerIndex, id);
    }
};


struct Player
{
    // Constructor
    Player(Vec2f startPos, uint8_t playerIndex)
    : position{startPos}, playerIndex{playerIndex}
    {};
    Player() {};

    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    float aimDirection = 0; // in Radians
    uint8_t score = 0;

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(position, velocity, playerIndex, aimDirection, score);
    }
};

struct GameSnapshot
{
    std::vector<Player> players;
    std::vector<Bullet> bullets;

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(players, bullets);
    }
};


#endif /* ENTITIES_HPP */