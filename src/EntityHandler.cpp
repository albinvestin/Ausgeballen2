#include "../inc/EntityHandler.hpp"
#include "../inc/Player.hpp"

// Spawn players?
EntityHandler::EntityHandler()
{
}

EntityHandler::~EntityHandler()
{
}

void EntityHandler::Update(int inputkeys)
{
    int player1Key = 2; // TODO; make DEFINE
    if (inputkeys == player1Key)
    {
        mP1.updatePos();
    }
    mP1.updateAimDirection();
}

Vec2 EntityHandler::GetP1Pos()
{
    return mP1.GetPos();
}
float EntityHandler::GetP1Aim()
{
    return mP1.GetAim();
}