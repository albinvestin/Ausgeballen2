#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include <SDL2/SDL.h>

Player::Player()
    : mPos(Vector2{100,100}), mVel(Vector2{1,1})
{
}

Player::~Player()
{
}

Vector2 Player::update()
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


