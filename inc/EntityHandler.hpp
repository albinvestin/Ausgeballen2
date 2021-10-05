#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
#include "Player.hpp"
class EntityHandler
{
private:
    Player mP1{};
public:
    EntityHandler();
    ~EntityHandler();
    void Update(int inputkeys);
    Vec2 GetP1Pos();
	float GetP1Aim();
};
#endif