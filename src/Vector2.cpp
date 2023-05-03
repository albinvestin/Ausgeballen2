#include "../inc/Vector2.hpp"
#include <cmath>
#include <iostream>

float Vec2Length(const Vec2f &v)
{
    return (float)sqrt(pow(v.x,2)+ pow(v.y,2));
}

Vec2f Vec2f::operator*=(float multiplier)
{
    x *= multiplier;
    y *= multiplier;
    return *this;
}

Vec2f operator *(float multiplier, const Vec2f &v)
{
    Vec2f result = v;
    result *= multiplier;
    return result;
}

Vec2f operator /(const Vec2f &v, float div)
{
    Vec2f result;
    result.x = v.x / div;
    result.y = v.y / div;
    return result;
}

Vec2f Vec2f::operator+=(const Vec2f &vec)
{
    // Use compound assignment
    x += vec.x;
    y += vec.y;
    return *this;
}
Vec2f Vec2f::operator-=(const Vec2f &vec)
{
    // Use compound assignment
    x -= vec.x;
    y -= vec.y;
    return *this;
}

float DotProduct(const Vec2f &v1, const Vec2f &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

Vec2f operator-(const Vec2f &lhs, const Vec2f &rhs)
{
    Vec2f result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

Vec2f Normalized(const Vec2f &v)
{
    float length = Vec2Length(v);
    return v / length;
}

// Assignment operator, would be equal to the default, but written explicitly.
// v2 = v1 calls assignment operator, same as "v2.operator=(v1);"
Vec2f& Vec2f::operator=(const Vec2f &other) 
{
    x = other.x;
    y = other.y;
    return *this; 
}

// Copy operator,  would be equal to the default
// Vec2f v3 = v1 calls copy constructor, same as "Vec2f v3(v1);"
Vec2f::Vec2f(const Vec2f &other) :
    x{other.x}, y{other.y}
{}
