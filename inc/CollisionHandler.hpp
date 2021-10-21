#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP
#include "../inc/EntityHandler.hpp"
#include "Vector2.hpp"

class CollisionHandler
{
public:
    CollisionHandler();
    ~CollisionHandler();
    void HandleCollisons(EntityHandler *entities);
};

float Squaredf(float x);
Vec2f ElasticCollision(Vec2f playerPos1, Vec2f playerPos2, Vec2f p1Vel, Vec2f p2Vel);



#endif /* COLLISIONHANDLER_HPP */
