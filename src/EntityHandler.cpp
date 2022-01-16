#include "../inc/EntityHandler.hpp"
//#include "../inc/Player.hpp"
#include "../inc/Entities.hpp"
#include "../inc/Constants.hpp"
#include "../inc/InputHandler.hpp"
#include "../inc/NetworkHandler.hpp"
#include <cmath>
#include <stdio.h>

// Spawn players?
EntityHandler::EntityHandler(NetworkHandler* networkHandler)
    : _networkHandler{networkHandler}
    // : _P1{Vec2f(MAP_WIDTH/4, MAP_HEIGHT/4)}, _P2{Vec2f((MAP_WIDTH*3)/4, MAP_HEIGHT/4)}
{
    _Players.reserve(2); // TODO Make number of players variable
    for (uint8_t index = 1; index <= 2; index++)
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

void EntityHandler::ServerUpdate(int inputkeys)
{
    std::vector<Player>::iterator itP = _Players.begin();
    while (itP != _Players.end())
    {
        Vec2f playerPos = UpdatePlayerPos((*itP).playerIndex);
        float playerAimDir = UpdateAimDirection((*itP).playerIndex);
        uint8_t playerIndex = (*itP).playerIndex;
        if (    (inputkeys == NETWORK_ACTION_SHOOT_P1 && playerIndex == 1)
             || (inputkeys == NETWORK_ACTION_SHOOT_P2 && playerIndex == 2)
             || (inputkeys == INPUT_P1SHOOT && playerIndex == 1))
        {
            // Spawn bullet
            // std::shared_ptr<Bullet> newBullet{new Bullet{playerAimDir, playerPos, playerIndex}};
            // Bullet newBullet{playerAimDir, playerPos, playerIndex}; // TODO: create object just once and refer by pointer. Remember to delete the pointer and free mem.
            Bullet b = {playerAimDir, playerPos, playerIndex};
            _ExistingBullets.push_back(b);
            Vec2f v = {AddRecoil((*itP).playerIndex)};
            // std::shared_ptr<Vec2f> newVel{new Vec2f(AddRecoil((*itP).playerIndex))};
            // Send reply to client of new bullet and recoil
            _networkHandler->S2CBulletRecoilPlayerIndex(b, v, playerIndex);
        }
        ++itP;
    }
    // Update position and remove out of bounds bullets
    // TODO Handle bullet removal sent to clients
    auto itB = _ExistingBullets.begin();
    while (itB != _ExistingBullets.end())
    {
        Vec2f bullpos = UpdateBulletPos(itB);
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
void EntityHandler::SetRecoilOfPlayer(Vec2f vel, uint8_t playerIndex)
{
    if (_Players[playerIndex-1].playerIndex != playerIndex)
    {
        printf("EntityHandler::SetRecoilOfPlayer ERROR\n");
    }
    else
    {
        _Players[playerIndex-1].velocity = vel;
    }
}

// Registers new bullet with unique pointer and returns that reference 
// const Bullet& EntityHandler::RegisterNewBullet(const Bullet& bullet) // TODO bad practice to share ref to unique pointer
// {
//     std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(bullet);
//     _ExistingBullets.push_back(newBullet);
//     return *(_ExistingBullets.back());
// }

// index starts at 1, TODO: check length of _Players before getting index
Vec2f EntityHandler::GetPlayerPos(int index) const
{
    if (_Players[index-1].playerIndex != index)
    {
        printf("EntityHandler::GetPlayerPos(int index) ERROR\n");
    }
    return _Players[index-1].position; 
}

// index starts at 1, TODO: check length of _Players before getting index
float EntityHandler::GetPlayerAim(int index) const
{
    if (_Players[index-1].playerIndex != index)
    {
        printf("EntityHandler::GetPlayerPos(int index) ERROR\n");
    }
    return _Players[index-1].aimDirection; 
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
        return _ExistingBullets[0].position;
    }
}

std::vector<Vec2f> EntityHandler::GetAllBulletPos() const
{
    std::vector<Vec2f> result{};
    auto it = _ExistingBullets.begin();
    while (it != _ExistingBullets.end())
    {
        result.push_back(it->position);
        ++it;
    }
    return result;
}

const std::vector<Bullet> & EntityHandler::GetAllBullets()
{
    return _ExistingBullets;
}

std::vector<Player>& EntityHandler::GetAllPlayers()
{
    return _Players;
}

Vec2f EntityHandler::UpdatePlayerPos(uint8_t playerIndex) // TODO take an iterator
{
    Vec2f _Position = _Players[playerIndex-1].position;
    Vec2f _Velocity = _Players[playerIndex-1].velocity;
    _Position.x += _Velocity.x;
    _Position.y += _Velocity.y;
    // Boarders handling
    if (_Position.x + 2*PLAYER_RADIUS > MAP_WIDTH) // Right side
    {
        // Move the amount past the wall back into the map.
        _Position.x -= (_Position.x + 2*PLAYER_RADIUS - MAP_WIDTH);
        _Velocity.x = -_Velocity.x;
    }
    else if (_Position.x < 0) // Left side
    {
        _Position.x -= _Position.x;
        _Velocity.x = -_Velocity.x;
    }
    if (_Position.y + 2*PLAYER_RADIUS > MAP_HEIGHT) // Bottom side
    {
        // Move the amount past the wall back into the map.
        _Position.y -= (_Position.y + 2*PLAYER_RADIUS - MAP_HEIGHT);
        _Velocity.y = -_Velocity.y;
    }
    else if (_Position.y < 0) // Top side
    {
        _Position.y -= _Position.y;
        _Velocity.y = -_Velocity.y;
    }

    // Decrease velocity
    if (Vec2Length(_Velocity) < 1)
    {
        _Velocity.x = 0;
        _Velocity.y = 0;
    }
    else
    {
        _Velocity *= PLAYER_FRICTION;
    }

    // Update the position and velocity
    _Players[playerIndex-1].position = _Position;
    _Players[playerIndex-1].velocity = _Velocity;

    return _Position;
}

Vec2f EntityHandler::AddRecoil(uint8_t playerIndex) // TODO take an iterator
{
    Vec2f _Velocity = _Players[playerIndex-1].velocity;
    float _AimDirection = _Players[playerIndex-1].aimDirection;
    _Velocity.x -= cos(_AimDirection) * PLAYER_RECOIL;
    _Velocity.y -= sin(_AimDirection) * PLAYER_RECOIL;
    _Players[playerIndex-1].velocity = _Velocity;
    return _Velocity;
}


float EntityHandler::UpdateAimDirection(uint8_t playerIndex) // TODO take an iterator
{
    float _AimDirection = _Players[playerIndex-1].aimDirection;
    _AimDirection += M_PI/30;
	if (_AimDirection > 2*M_PI)
	{
		_AimDirection = 0;
	}
    _Players[playerIndex-1].aimDirection = _AimDirection;
    return _AimDirection;
}

Vec2f EntityHandler::UpdateBulletPos(std::vector<Bullet>::iterator bullet)
{
    return bullet->position += bullet->velocity;
}

std::vector<Bullet>::const_iterator EntityHandler::RemoveBulletFromIt(std::vector<Bullet>::const_iterator itB)
{
    printf("TRYING TO REMOVE BULLET FROM IT\n");
    // TODO make check if this is valid?
    return _ExistingBullets.erase(itB);
}

void EntityHandler::AddNewBullet(const Bullet& b)
{
    _ExistingBullets.push_back(b);
}