#include "../inc/EntityHandler.hpp"
#include "../inc/Player.hpp"
#include "../inc/Constants.hpp"
#include <cmath>
#include <stdio.h>


Bullet::Bullet(float aimDirection, Vec2f playerPos)
    : _Position{playerPos},
      _Velocity{(float)(BULLET_INIT_VEL * cos(aimDirection)),
                (float)(BULLET_INIT_VEL * sin(aimDirection))}
{
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
    : _P1{Vec2f(MAP_WIDTH/4, MAP_HEIGHT/4)}, _P2{Vec2f((MAP_WIDTH*3)/4, MAP_HEIGHT/4)}
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
    if (inputkeys == INPUT_P1SHOOT) // TODO: allow multiple keys to be pressed
    {
        // Spawn bullet
        Bullet newBullet{p1Aim, p1Pos}; // TODO: create object just once and refer by pointer. Remember to delete the pointer and free mem.
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

// index starts at 1
Vec2f EntityHandler::GetPlayerPos(int index)
{
    switch (index)
    {
    case 1:
        return _P1.GetPos();
    case 2:
        return _P2.GetPos();
    default:
        printf("No Player Specified\n");
        return _P1.GetPos();
        break;
    }
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
        Vec2f outOfBounds{-1000, -1000}; // TODO: make a better solution if no bullet exists
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
