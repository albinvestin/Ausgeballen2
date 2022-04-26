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
    _players.reserve(2); // TODO Make number of players variable
    for (uint8_t index = 1; index <= 2; index++)
    {
        Vec2f spawningPos{(MAP_WIDTH*index)/4, (MAP_HEIGHT*index)/4};
        Player NewPlayer{spawningPos, index};
        _players.push_back(NewPlayer);
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

void EntityHandler::MoveAllObjects()
{
    std::vector<Player>::iterator itP = _players.begin();
    while (itP != _players.end())
    {
        UpdatePlayerPos((*itP).playerIndex);
        UpdateAimDirection((*itP).playerIndex);
        ++itP;
    }

    // Update position and remove out of bounds bullets
    // TODO Handle bullet removal sent to clients = Should this be handled by server only?
    auto itB = _existingBullets.begin();
    while (itB != _existingBullets.end())
    {
        Vec2f bullpos = UpdateBulletPos(itB);
        if (isOutOfBounds(bullpos))
        {
            itB = _existingBullets.erase(itB);
        }
        else
        {
            ++itB;
        }
    }
}

// Handle shoot inputs and spawns bullets
void EntityHandler::ServerCheckAndHandleShoot(int inputkeys)
{
    if (inputkeys != INPUT_P1SHOOT && inputkeys != NETWORK_ACTION_SHOOT_P2)
    {
        return;
    }
    Player* player;
    if (inputkeys == INPUT_P1SHOOT)
    {
        player = &_players[0];
    }
    else if (inputkeys == NETWORK_ACTION_SHOOT_P2)
    {
        player = &_players[1];
    }
    else
    {
        return; // Unnecessary?
    }
    // Spawn bullet
    Bullet b = {player->aimDirection, player->position, player->playerIndex, _nextBulletID};
    _nextBulletID++; // Overflow allowed
    _existingBullets.push_back(b);
    Vec2f v = {AddRecoil(player->playerIndex)};
}

void EntityHandler::UpdateClients()
{
    //Send snapshot of game to all clients
    GameSnapshot gs{_players, _existingBullets};
    _networkHandler->S2CGameSnapshot(gs);
}

// index starts at 1, TODO: check length of _Players before getting index
void EntityHandler::SetRecoilOfPlayer(Vec2f vel, uint8_t playerIndex)
{
    if (_players[playerIndex-1].playerIndex != playerIndex)
    {
        printf("EntityHandler::SetRecoilOfPlayer ERROR\n");
    }
    else
    {
        _players[playerIndex-1].velocity = vel;
    }
}

// index starts at 1, TODO: check length of _Players before getting index
Vec2f EntityHandler::GetPlayerPos(int index) const
{
    if (_players[index-1].playerIndex != index)
    {
        printf("EntityHandler::GetPlayerPos(int index) ERROR\n");
    }
    return _players[index-1].position; 
}

// index starts at 1, TODO: check length of _Players before getting index
float EntityHandler::GetPlayerAim(int index) const
{
    if (_players[index-1].playerIndex != index)
    {
        printf("EntityHandler::GetPlayerPos(int index) ERROR\n");
    }
    return _players[index-1].aimDirection; 
}

Vec2f EntityHandler::GetBullet1Pos()
{
    if (_existingBullets.empty())
    {
        Vec2f outOfBounds{-1000, -1000}; // TODO: make a better solution if no bullet exists
        return outOfBounds;
    }
    else
    {
        return _existingBullets[0].position;
    }
}

std::vector<Vec2f> EntityHandler::GetAllBulletPos() const
{
    std::vector<Vec2f> result{};
    auto it = _existingBullets.begin();
    while (it != _existingBullets.end())
    {
        result.push_back(it->position);
        ++it;
    }
    return result;
}

const std::vector<Bullet> & EntityHandler::GetAllBullets()
{
    return _existingBullets;
}

std::vector<Player>& EntityHandler::GetAllPlayers()
{
    return _players;
}

Vec2f EntityHandler::UpdatePlayerPos(uint8_t playerIndex) // TODO take an iterator
{
    Vec2f _Position = _players[playerIndex-1].position;
    Vec2f _Velocity = _players[playerIndex-1].velocity;
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
    _players[playerIndex-1].position = _Position;
    _players[playerIndex-1].velocity = _Velocity;

    return _Position;
}

Vec2f EntityHandler::AddRecoil(uint8_t playerIndex) // TODO take an iterator
{
    Vec2f _Velocity = _players[playerIndex-1].velocity;
    float _AimDirection = _players[playerIndex-1].aimDirection;
    _Velocity.x -= cos(_AimDirection) * PLAYER_RECOIL;
    _Velocity.y -= sin(_AimDirection) * PLAYER_RECOIL;
    _players[playerIndex-1].velocity = _Velocity;
    return _Velocity;
}


float EntityHandler::UpdateAimDirection(uint8_t playerIndex) // TODO take an iterator
{
    float _AimDirection = _players[playerIndex-1].aimDirection;
    _AimDirection += M_PI/30;
    if (_AimDirection > 2*M_PI)
    {
        _AimDirection = 0;
    }
    _players[playerIndex-1].aimDirection = _AimDirection;
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
    return _existingBullets.erase(itB);
}

void EntityHandler::HandleNetworkShoot(Bullet& b, Vec2f& newVel, uint8_t playerIndex, Vec2f& playerPos)
{

    _existingBullets.push_back(b);
    _players[playerIndex-1].position = playerPos;
    SetRecoilOfPlayer(newVel, playerIndex);
}

void EntityHandler::HandleNetworkGameSnapshot(const GameSnapshot& gs)
{
    _players = gs.players;
    _existingBullets = gs.bullets;
}