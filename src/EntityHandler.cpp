#include "../inc/EntityHandler.hpp"
#include "../inc/Player.hpp"
#include "../inc/Constants.hpp"
#include <cmath>


Bullet::Bullet(float aimDirection, Vec2f playerPos)
{
    _Velocity.x = BULLET_INIT_VEL * cos(aimDirection);
    _Velocity.y = BULLET_INIT_VEL * sin(aimDirection);
    _Position.x = playerPos.x;
    _Position.y = playerPos.y;
}

Bullet::~Bullet()
{
}

Vec2f Bullet::UpdatePos()
{
    _Position.x += _Velocity.x;
    _Position.y += _Velocity.y;
    return _Position;
}



// Spawn players?
EntityHandler::EntityHandler()
{
}

EntityHandler::~EntityHandler()
{
}

bool isOutOfBounds(Vec2f position)
{
    // TODO access defined map size
    if (position.x > MAP_WIDTH || position.x < 0)
    {
        return true;
    }
    else if (position.y > MAP_HEIGHT || position.y < 0)
    {
        return true;
    }
    return false;
}

void EntityHandler::Update(int inputkeys)
{
    Vec2f p1Pos = _P1.updatePos();
    float p1Aim = _P1.updateAimDirection();
    if (inputkeys == INPUT_P1SHOOT) // TODO: allow _ultiple keys to be pressed
    {
        // Spawn bullet
        Bullet newBullet{p1Aim, p1Pos}; // TODO: create object just once and refer by pointer. Remember to delete the pointer and free _em.
        _ExistingBullets.push_back(newBullet);
        _P1.AddRecoil();
    }
    std::vector<Bullet>::iterator it = _ExistingBullets.begin();
    while (it != _ExistingBullets.end())
    {
        Vec2f bullpos = (*it).UpdatePos();
        if (isOutOfBounds(bullpos))
        {
            it = _ExistingBullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Vec2f EntityHandler::GetP1Pos()
{
    return _P1.GetPos();
}

float EntityHandler::GetP1Aim()
{
    return _P1.GetAim();
}

Vec2f Bullet::GetPos()
{
    return _Position;
}

Vec2f EntityHandler::GetBullet1Pos()
{
    if (_ExistingBullets.empty())
    {
        Vec2f outOfBounds{-1000, -1000}; // TODO: _ake a better solution if no bullet exists
        return outOfBounds;
    }
    else
    {
        return _ExistingBullets.at(0).GetPos();
    }
}

std::vector<Vec2f> EntityHandler::GetAllBulletPos()
{
    std::vector<Vec2f> result{};
    std::vector<Bullet>::iterator it = _ExistingBullets.begin();
    while (it != _ExistingBullets.end())
    {
        result.push_back((*it).GetPos());
        ++it;
    }
    return result;
}
