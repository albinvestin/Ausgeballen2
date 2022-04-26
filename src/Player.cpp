#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <cmath>
#include <iostream>
#include <stdint.h>

Player::Player(Vec2f startPos, int playerIndex)
    : _position(startPos), _Velocity(Vec2f{0,0}), _playerIndex{playerIndex}
{
}

Player::~Player()
{
}

Vec2f Player::UpdatePos()
{
    _position.x += _Velocity.x;
    _position.y += _Velocity.y;
    // Boarders handling
    if (_position.x + 2*PLAYER_RADIUS > MAP_WIDTH) // Right side
    {
        // Move the amount past the wall back into the map.
        _position.x -= (_position.x + 2*PLAYER_RADIUS - MAP_WIDTH);
        _Velocity.x = -_Velocity.x;
    }
    else if (_position.x < 0) // Left side
    {
        _position.x -= _position.x;
        _Velocity.x = -_Velocity.x;
    }
    if (_position.y + 2*PLAYER_RADIUS > MAP_HEIGHT) // Bottom side
    {
        // Move the amount past the wall back into the map.
        _position.y -= (_position.y + 2*PLAYER_RADIUS - MAP_HEIGHT);
        _Velocity.y = -_Velocity.y;
    }
    else if (_position.y < 0) // Top side
    {
        _position.y -= _position.y;
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

    return _position;
}

void Player::AddRecoil()
{
    _Velocity.x -= cos(_AimDirection) * PLAYER_RECOIL;
    _Velocity.y -= sin(_AimDirection) * PLAYER_RECOIL;
}


float Player::updateAimDirection()
{
    _AimDirection += M_PI/30;
    if (_AimDirection > 2*M_PI)
    {
        _AimDirection = 0;
    }
    return _AimDirection;
}

Vec2f Player::GetPos() const
{
    return _position;
}

float Player::GetAim() const
{
    return _AimDirection;
}

int Player::GetPlayerIndex() const
{
    return _playerIndex;
}

Vec2f Player::AddVelocity(Vec2f vel)
{
    _Velocity += vel;
    return _Velocity;
}

Vec2f Player::GetVelocity()
{
    return _Velocity;
}

Vec2f Player::SetVelocity(Vec2f v)
{
    _Velocity = v;
    return _Velocity;
}

void Player::AddToPosition(Vec2f v)
{
    _position += v;
}

void Player::AddOneToScore()
{
    _score += 1;
    printf("%d\n", _score);
}

uint8_t Player::GetScore()
{
    return _score;
}
