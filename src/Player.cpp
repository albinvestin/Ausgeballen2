#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#if defined(__APPLE__) || defined(__linux__)
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

Vec2 Player::updatePos()
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


float Player::updateAimDirection()
{
    mAimDirection += 3.14/3200;
	if (mAimDirection > 2*3.14)
	{
		mAimDirection = 0;
	}
    return mAimDirection;
}
