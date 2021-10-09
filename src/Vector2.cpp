#include "../inc/Vector2.hpp"
#include <cmath>
int Vec2Length(Vec2 v)
{
    return (int)sqrt(pow(v.x,2)+ pow(v.y,2));
}
float Vec2Length(Vec2f v)
{
    return (float)sqrt(pow(v.x,2)+ pow(v.y,2));
}
Vec2f Vec2f::operator*=(float multiplier)
{
    x *= multiplier;
    y *= multiplier;
    return *this;
}