#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <cmath>
#include <iostream>

Player::Player()
    : mPos(Vec2{100,100}), mVel(Vec2f{0,0})
{
}

Player::~Player()
{
}

Vec2 Player::updatePos()
{
    mPos.x += mVel.x;
    mPos.y += mVel.y;
    // Boarders handling
    if (mPos.x > 640 || mPos.x < 10)
    {
        if (mPos.x < 10)
        {
            mPos.x += (10 - mPos.x);
        }
        else
        {
            mPos.x -= (mPos.x - 640);
        }
        mVel.x = -mVel.x;
    }
    if (mPos.y > 480 || mPos.y < 10)
    {
        if (mPos.y < 10)
        {
            mPos.y += (10 - mPos.y);
        }
        else
        {
            mPos.y -= (mPos.y - 480);
        }
        mVel.y = -mVel.y;
    }

    // Decrease velocity
    if (Vec2Length(mVel) > 0)
    {
        printf("Vel (%f,%f), ", mVel.x, mVel.y);
        printf("VelLength %f\n", Vec2Length(mVel));
    }
    if (Vec2Length(mVel) < 1)
    {
        mVel.x = 0;
        mVel.y = 0;
    }
    else
    {
        mVel *= 0.95;
    }

    return mPos;
}

void Player::AddRecoil()
{
    mVel.x -= cos(mAimDirection) * mRecoilSize;
    mVel.y -= sin(mAimDirection) * mRecoilSize;
}


float Player::updateAimDirection()
{
    mAimDirection += M_PI/30;
	if (mAimDirection > 2*M_PI)
	{
		mAimDirection = 0;
	}
    return mAimDirection;
}

Vec2 Player::GetPos()
{
    return mPos;
}

float Player::GetAim()
{
    return mAimDirection;
}
