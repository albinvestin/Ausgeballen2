#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

Player::Player()
    : mPos(Vec2{100,100}), mVel(Vec2{1,1})
{
}

Player::~Player()
{
}

Vec2 Player::update()
{
    mPos.x += mVel.x;
    mPos.y += mVel.y;
    if (mPos.x > 640 || mPos.x < 10)
    {
        mVel.x = -mVel.x;
    }
    if (mPos.y > 480 || mPos.y < 10)
    {
        mVel.y = -mVel.y;
    }
    return mPos;
}


