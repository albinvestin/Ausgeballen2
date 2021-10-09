#ifndef VECTOR2_HPP
#define VECTOR2_HPP
struct Vec2
{
    int x, y = 0;
};
struct Vec2f
{
    float x, y = 0;
    Vec2f operator*=(float multiplier);
};
int Vec2Length(Vec2 v);
float Vec2Length(Vec2f v);
#endif
