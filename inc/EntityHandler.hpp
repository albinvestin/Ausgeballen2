#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
#include "Player.hpp"
#include <vector>

class Bullet
{
private:
    Vec2f mVelocity;
    int mInitialVel = 20;
    Vec2f mPosition;
public:
    Bullet(float aimDirection, Vec2f playerPos);
    ~Bullet();
    Vec2f UpdatePos();
    Vec2f GetPos();
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
    Vec2f GetP1Pos();
	float GetP1Aim();
    Vec2f GetBullet1Pos();
    std::vector<Vec2f> GetAllBulletPos();
};
#endif