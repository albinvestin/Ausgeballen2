#include "../inc/EntityHandler.hpp"
#include "../inc/Player.hpp"
#include "../inc/Constants.hpp"
#include "../inc/InputHandler.hpp"
#include <cmath>
#include <stdio.h>


Bullet::Bullet(float aimDirection, Vec2f playerPos, int playerIndex)
    : _Position{playerPos},
      _Velocity{(float)(BULLET_INIT_VEL * cos(aimDirection)),
                (float)(BULLET_INIT_VEL * sin(aimDirection))},
      _playerIndex{playerIndex}
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

Vec2f Bullet::GetVelocity()
{
    return _Velocity;
}



// Spawn players?
EntityHandler::EntityHandler()
    // : _P1{Vec2f(MAP_WIDTH/4, MAP_HEIGHT/4)}, _P2{Vec2f((MAP_WIDTH*3)/4, MAP_HEIGHT/4)}
{
    _Players.reserve(2); // TODO Make number of players variable
    for (int index = 1; index <= 2; index++)
    {
        Vec2f spawningPos{(MAP_WIDTH*index)/4, (MAP_HEIGHT*index)/4};
        Player NewPlayer{spawningPos, index};
        _Players.push_back(NewPlayer);
    }
    // printf("Entity player adress: %d\n", &_Players);
    // printf("Entity player adress: %d\n", &_Players[0] );
}

EntityHandler::~EntityHandler()
{
}

bool isOutOfBounds(Vec2f position)
{
    // TODO access defined map size
    if (position.x + 2*BULLET_RADIUS > MAP_WIDTH || position.x < 0)
    {
        return true;
    }
    else if (position.y + 2*BULLET_RADIUS  > MAP_HEIGHT || position.y < 0)
    {
        return true;
    }
    return false;
}

void EntityHandler::Update(int inputkeys)
{
    std::vector<Player>::iterator itP = _Players.begin();
    while (itP != _Players.end())
    {
        Vec2f playerPos = (*itP).UpdatePos();
        float playerAimDir = (*itP).updateAimDirection();
        int playerIndex = (*itP).GetPlayerIndex();
        if (    (inputkeys == INPUT_P1SHOOT && playerIndex == 1)
             || (inputkeys == INPUT_P2SHOOT && playerIndex == 2))
        {
            // Spawn bullet
            Bullet newBullet{playerAimDir, playerPos, playerIndex}; // TODO: create object just once and refer by pointer. Remember to delete the pointer and free mem.
            _ExistingBullets.push_back(newBullet);
            (*itP).AddRecoil();
        }
        ++itP;
    }

    std::vector<Bullet>::iterator itB = _ExistingBullets.begin();
    while (itB != _ExistingBullets.end())
    {
        Vec2f bullpos = (*itB).UpdatePos();
        if (isOutOfBounds(bullpos))
        {
            itB = _ExistingBullets.erase(itB);
        }
        else
        {
            ++itB;
        }
    }
}

// index starts at 1, TODO: check length of _Players before getting index
Vec2f EntityHandler::GetPlayerPos(int index)
{
    if (_Players[index-1].GetPlayerIndex() != index)
    {
        printf("EntityHandler::GetPlayerPos(int index) ERROR\n");
    }
    return _Players[index-1].GetPos(); 
}

// index starts at 1, TODO: check length of _Players before getting index
float EntityHandler::GetPlayerAim(int index)
{
    if (_Players[index-1].GetPlayerIndex() != index)
    {
        printf("EntityHandler::GetPlayerPos(int index) ERROR\n");
    }
    return _Players[index-1].GetAim(); 
}

Vec2f Bullet::GetPos()
{
    return _Position;
}

int Bullet::GetPlayerIndex()
{
    return _playerIndex;
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

std::vector<Bullet>* EntityHandler::GetAllBullets()
{
    return &_ExistingBullets;
}

std::vector<Player>* EntityHandler::GetAllPlayers()
{
    return &_Players;
}
