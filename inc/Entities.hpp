#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include "Vector2.hpp"
#include "Constants.hpp"
// #include "SerializeBaseData.hpp"
// #include <cereal/archives/portable_binary.hpp>
// #include <cereal/types/polymorphic.hpp>
// #include <cereal/access.hpp> // For LoadAndConstruct
#include <cmath>

struct Bullet //: public SerializeBaseData
{
    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    // Constructor
    Bullet() {}
    // Bullet(const Bullet &other) = default; // Copy operator
    Bullet(float aimDirection, Vec2f playerPos, uint8_t playerIndex)
    : position{playerPos},
      velocity{(float)(BULLET_INIT_VEL * cos(aimDirection)),
               (float)(BULLET_INIT_VEL * sin(aimDirection))},
      playerIndex{playerIndex}
    {}

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(position, velocity, playerIndex);
    }
    // void SerializeBase() {};
    // template<class Archive> void serialize(Archive & ar)
    // {
    //     ar(position, velocity, playerIndex);
    // }
    // template <class Archive> static void load_and_construct(Archive &ar, cereal::construct<Vec2f> &construct)
    // {
    //     Vec2f position, velocity;
    //     uint8_t playerIndex;
    //     ar(position, velocity, playerIndex);
    //     construct(position, velocity, playerIndex);
    // }
};
// CEREAL_REGISTER_TYPE(Bullet)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(SerializeBaseData, Bullet)

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