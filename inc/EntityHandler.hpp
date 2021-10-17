#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
#include "Player.hpp"
#include <vector>

class Bullet
{
private:
    Vec2f _Position;
    Vec2f _Velocity;
    int _playerIndex; // starts at 1
public:
    Bullet(float aimDirection, Vec2f playerPos, int playerIndex);
    ~Bullet();
    Vec2f UpdatePos();
    Vec2f GetPos();
    int GetPlayerIndex();
    Vec2f GetVelocity();
};

class EntityHandler
{
private:
    // Player _P1;
    // Player _P2;
    std::vector<Player> _Players; // Make references to players instead of pass by value.
    std::vector<Bullet> _ExistingBullets;
public:
    EntityHandler();
    ~EntityHandler();
    void Update(int inputkeys);
    Vec2f GetPlayerPos(int index); // Index starts at 1
    float GetPlayerAim(int index);
    Vec2f GetBullet1Pos();
    std::vector<Vec2f> GetAllBulletPos();
    std::vector<Bullet>* GetAllBullets();
    std::vector<Player>* GetAllPlayers();
};
#endif