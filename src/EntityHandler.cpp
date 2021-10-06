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

bool isOutOfBounds(Vec2 position)
{
    // TODO access defined Map size
    if (position.x > 600 || position.x < 0)
    {
        return true;
    }
    else if (position.y > 400 || position.y < 0)
    {
        return true;
    }
    return false;
}

void EntityHandler::Update(int inputkeys)
{
    int player1Key = 2; // TODO; make DEFINE
    int player1Bullet = 3; // TODO; make DEFINE
    Vec2 p1Pos = mP1.GetPos();
    if (inputkeys == player1Key)
    {
        p1Pos = mP1.updatePos();
    }
    float p1Aim = mP1.updateAimDirection();
    if (inputkeys == player1Bullet) // TODO: allow multiple keys to be pressed
    {
        // Spawn bullet
        Bullet newBullet{p1Aim, p1Pos}; // TODO: create object just once and refer by pointer. Remember to delete the pointer and free mem.
        mExistingBullets.push_back(newBullet);
    }
    std::vector<Bullet>::iterator it = mExistingBullets.begin();
    while (it != mExistingBullets.end())
    {
        Vec2 bullpos = (*it).UpdatePos();
        if (isOutOfBounds(bullpos))
        {
            it = mExistingBullets.erase(it);
        }
        else
        {
            ++it;
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

std::vector<Vec2> EntityHandler::GetAllBulletPos()
{
    std::vector<Vec2> result{};
    std::vector<Bullet>::iterator it = mExistingBullets.begin();
    while (it != mExistingBullets.end())
    {
        result.push_back((*it).GetPos());
        ++it;
    }
    return result;
}