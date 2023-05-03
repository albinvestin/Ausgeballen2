#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP
#include "../inc/EntityHandler.hpp"
#include "Vector2.hpp"

class CollisionHandler
{
public:
    CollisionHandler();
    ~CollisionHandler();
    void HandleCollisons(EntityHandler &entities);
};

// float Squaredf(const float x);
// Vec2f ElasticCollision(const Vec2f &playerPos1, const Vec2f &playerPos2, const Vec2f &p1Vel, const Vec2f &p2Vel);



#endif /* COLLISIONHANDLER_HPP */
