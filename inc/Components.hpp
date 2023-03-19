#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "Vector2.hpp"
#include "Texture.hpp"

class Component
{
public:
    bool has = false;
    virtual ~Component() {}
};

class CTransform : public Component
{
public:
    Vec2f position{0, 0};
    Vec2f velocity{0, 0};

    CTransform() {}
    CTransform(const Vec2f & p)
        : position(p) {}
    CTransform(const Vec2f & p, const Vec2f & vel)
        : position(p), velocity(vel) {}
};

class CPlayer : public Component
{
public:
    uint8_t playerIndex  = 255;
    float   aimDirection = 0; // in Radians
    uint8_t score        = 0;

    CPlayer() {}
    CPlayer(uint8_t playerIndex, float aimDirection)
        : playerIndex(playerIndex), aimDirection(aimDirection) {}
};

class CBullet : public Component
{
public:
    uint8_t playerIndex = 255;

    CBullet() {}
    CBullet(uint8_t playerIndex)
        : playerIndex(playerIndex) {}
};

typedef std::tuple<CTransform, CPlayer, CBullet> AvailableComponents;

#endif /* COMPONENTS_HPP */