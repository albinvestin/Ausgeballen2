#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Vector2.hpp"
#include "Texture.hpp"

struct Component
{
    bool has = false;
    virtual ~Component() {}
};

// Components should be small and grouped with members that are accessed together.

struct CTransform : public Component
{
    Vec2f position{0, 0};
    Vec2f velocity{0, 0};

    CTransform() {}
    CTransform(const Vec2f & p)
        : position(p) {}
    CTransform(const Vec2f & p, const Vec2f & vel)
        : position(p), velocity(vel) {}
};

struct CCannon : public Component
{
    float   aimDirection = 0; // in Radians
    bool    isShooting = false;

    CCannon() {}
    CCannon(float aimDirection)
        : aimDirection(aimDirection) {}
};

struct CScore : public Component
{
    uint8_t score = 0;

    CScore() {}
};

struct CIndex : public Component
{
    uint8_t playerIndex = 255;

    CIndex() {}
    CIndex(uint8_t playerIndex)
        : playerIndex(playerIndex) {}
};

typedef std::tuple<CTransform, CCannon, CScore, CIndex> AvailableComponents;

#endif /* COMPONENTS_HPP */