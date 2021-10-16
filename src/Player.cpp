#include "../inc/Player.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/Constants.hpp"
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <cmath>
#include <iostream>

Player::Player(Vec2f startPos)
    : _Pos(startPos), _Vel(Vec2f{0,0})
{
}

Player::~Player()
{
}

Vec2f Player::updatePos()
{
    _Pos.x += _Vel.x;
    _Pos.y += _Vel.y;
    // Boarders handling
    if (_Pos.x + 2*PLAYER_RADIUS > MAP_WIDTH) // Right side
    {
        // Move the amount past the wall back into the map.
        _Pos.x -= (_Pos.x + 2*PLAYER_RADIUS - MAP_WIDTH);
        _Vel.x = -_Vel.x;
    }
    else if (_Pos.x < 0) // Left side
    {
        _Pos.x -= _Pos.x;
        _Vel.x = -_Vel.x;
    }
    if (_Pos.y + 2*PLAYER_RADIUS > MAP_HEIGHT) // Bottom side
    {
        // Move the amount past the wall back into the map.
        _Pos.y -= (_Pos.y + 2*PLAYER_RADIUS - MAP_HEIGHT);
        _Vel.y = -_Vel.y;
    }
    else if (_Pos.y < 0) // Top side
    {
        _Pos.y -= _Pos.y;
        _Vel.y = -_Vel.y;
    }

    // Decrease velocity
    if (Vec2Length(_Vel) < 1)
    {
        _Vel.x = 0;
        _Vel.y = 0;
    }
    else
    {
        _Vel *= PLAYER_FRICTION;
    }

    return _Pos;
}

void Player::AddRecoil()
{
    _Vel.x -= cos(_AimDirection) * PLAYER_RECOIL;
    _Vel.y -= sin(_AimDirection) * PLAYER_RECOIL;
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

Vec2f Player::GetPos()
{
    return _Pos;
}

float Player::GetAim()
{
    return _AimDirection;
}
