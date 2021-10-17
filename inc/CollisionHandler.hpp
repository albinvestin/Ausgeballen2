#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP
#include "../inc/EntityHandler.hpp"

class CollisionHandler
{
private:
    bool CollisionTwoCircles(Vec2f pos1, int radius1, Vec2f pos2, int radius2);
public:
    CollisionHandler();
    ~CollisionHandler();
    bool CheckCollisons(EntityHandler *entities);
};




#endif /* COLLISIONHANDLER_HPP */
