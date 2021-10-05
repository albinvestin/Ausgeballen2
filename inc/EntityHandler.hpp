#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
#include "Player.hpp"
#include <vector>

class Bullet
{
private:
    Vec2 mVelocity;
    int mInitialVel = 2;
    Vec2 mPosition;
public:
    Bullet(float aimDirection, Vec2 playerPos);
    ~Bullet();
    Vec2 UpdatePos();
    Vec2 GetPos();
};

class EntityHandler
{
private:
    Player mP1{};
    std::vector<Bullet> mExistingBullets;
public:
    EntityHandler();
    ~EntityHandler();
    void Update(int inputkeys);
    Vec2 GetP1Pos();
	float GetP1Aim();
    Vec2 GetBullet1Pos();
};
#endif