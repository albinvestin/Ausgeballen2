#ifndef VECTOR2_HPP
#define VECTOR2_HPP
// #include "SerializeBaseData.hpp"
#include "Constants.hpp"
// #include <cereal/archives/portable_binary.hpp>
// #include <cereal/types/polymorphic.hpp>
// #include <cereal/access.hpp> // For LoadAndConstruct

struct Vec2f //: public SerializeBaseData
{
    float x, y = 0;
    Vec2f(float x, float y) : x{x}, y{y} {}
    Vec2f() : x{0}, y{0} {}
    
    Vec2f& operator=(const Vec2f &other); // Assignment operator
    // v2 = v1 calls assignment operator, same as "v2.operator=(v1);"
    Vec2f(const Vec2f &other); // Copy operator
    Vec2f operator*=(const float multiplier);
    Vec2f operator+=(const Vec2f &vec);

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(x, y);
    }
    // void SerializeBase() {};
    // template<class Archive> void serialize(Archive &ar)
    // {
    //     ar(x, y);
    // }
    // template <class Archive> static void load_and_construct(Archive &ar, cereal::construct<Vec2f> &construct)
    // {
    //     float x, y;
    //     ar(x, y);
    //     construct(x, y);
    // }
};
// CEREAL_REGISTER_TYPE(Vec2f)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(SerializeBaseData, Vec2f)

Vec2f operator-(const Vec2f &lhs, const Vec2f &rhs);
Vec2f operator*(float multiplier, const Vec2f &v);
Vec2f operator/(const Vec2f &v, float divider);
float Vec2Length(const Vec2f &v);
float DotProduct(const Vec2f &v1, const Vec2f &v2);
Vec2f Normalized(const Vec2f &v);
#endif
