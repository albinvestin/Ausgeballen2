#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
#include "Player.hpp"
#include <vector>

class Bullet
{
private:
    Vec2f _Position;
    Vec2f _Velocity;
public:
    Bullet(float aimDirection, Vec2f playerPos);
    ~Bullet();
    Vec2f UpdatePos();
    Vec2f GetPos();
};

class EntityHandler
{
private:
    Player _P1;
    Player _P2;
    std::vector<Bullet> _ExistingBullets;
public:
    EntityHandler();
    ~EntityHandler();
    void Update(int inputkeys);
    Vec2f GetPlayerPos(int index); // Index starts at 1
	float GetP1Aim();
    Vec2f GetBullet1Pos();
    std::vector<Vec2f> GetAllBulletPos();
};
#endif