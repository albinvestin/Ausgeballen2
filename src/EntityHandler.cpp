#include "../inc/EntityHandler.hpp"
#include "../inc/Player.hpp"
#include <cmath>



Bullet::Bullet(float aimDirection, Vec2 playerPos)
{
    mVelocity.x = mInitialVel * cos(aimDirection);
    mVelocity.y = mInitialVel * sin(aimDirection);
    mPosition.x = playerPos.x;
    mPosition.y = playerPos.y;
}

Bullet::~Bullet()
{
}

Vec2 Bullet::UpdatePos()
{
    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    return mPosition;
}



// Spawn players?
EntityHandler::EntityHandler()
{
}

EntityHandler::~EntityHandler()
{
}

void EntityHandler::Update(int inputkeys)
{
    int player1Key = 2; // TODO; make DEFINE
    int player1Bullet = 3; // TODO; make DEFINE
    Vec2 p1Pos;
    if (inputkeys == player1Key)
    {
        p1Pos = mP1.updatePos();
    }
    float p1Aim = mP1.updateAimDirection();
    if (inputkeys == player1Bullet) // TODO: allow multiple keys to be pressed
    {
        // Spawn bullet
        Bullet newBullet{p1Aim, p1Pos};
        mExistingBullets.push_back(newBullet);
    }
    if (!mExistingBullets.empty())
    {
        for (int i = 0; i < mExistingBullets.size(); i++)
        {
            mExistingBullets.at(i).UpdatePos();
        }
        
    }
}

Vec2 EntityHandler::GetP1Pos()
{
    return mP1.GetPos();
}

float EntityHandler::GetP1Aim()
{
    return mP1.GetAim();
}

Vec2 Bullet::GetPos()
{
    return mPosition;
}

Vec2 EntityHandler::GetBullet1Pos()
{
    if (mExistingBullets.empty())
    {
        Vec2 outOfBounds{-1000, -1000}; // TODO: make a better solution if no bullet exists
        return outOfBounds;
    }
    else
    {
        return mExistingBullets.at(0).GetPos();
    }
}