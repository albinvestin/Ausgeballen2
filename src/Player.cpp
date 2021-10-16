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

Player::Player()
    : _Pos(Vec2f{100,100}), _Vel(Vec2f{0,0})
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
    if (_Pos.x > MAP_WIDTH || _Pos.x < PLAYER_DIAMETER)
    {
        if (_Pos.x < PLAYER_DIAMETER)
        {
            _Pos.x += (PLAYER_DIAMETER - _Pos.x);
        }
        else
        {
            _Pos.x -= (_Pos.x - MAP_WIDTH);
        }
        _Vel.x = -_Vel.x;
    }
    if (_Pos.y > MAP_HEIGHT || _Pos.y < PLAYER_DIAMETER)
    {
        if (_Pos.y < PLAYER_DIAMETER)
        {
            _Pos.y += (PLAYER_DIAMETER - _Pos.y);
        }
        else
        {
            _Pos.y -= (_Pos.y - MAP_HEIGHT);
        }
        _Vel.y = -_Vel.y;
    }

    // Decrease velocity
    if (Vec2Length(_Vel) > 0)
    {
        printf("Vel (%f,%f), ", _Vel.x, _Vel.y);
        printf("VelLength %f\n", Vec2Length(_Vel));
    }
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
