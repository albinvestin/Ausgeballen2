#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include "Vector2.hpp"
#include "Constants.hpp"
#include "Components.hpp"
#include <vector>
#include <cmath>
#include <tuple>

struct Bullet
{
    // Constructor
    Bullet() {}
    // Bullet(const Bullet &other) = default; // Copy operator
    Bullet(float aimDirection, Vec2f playerPos, uint8_t playerIndex, uint16_t id)
    : position{playerPos},
      velocity{(float)(BULLET_INIT_VEL * cos(aimDirection)),
               (float)(BULLET_INIT_VEL * sin(aimDirection))},
      playerIndex{playerIndex},
      id{id}
    {}

    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    uint16_t id; // Not needed?

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(position, velocity, playerIndex, id);
    }
};


struct Player
{
    // Constructor
    Player(Vec2f startPos, uint8_t playerIndex)
    : position{startPos}, playerIndex{playerIndex}
    {};
    Player() {};

    // Data
    Vec2f position;
    Vec2f velocity;
    uint8_t playerIndex;
    float aimDirection = 0; // in Radians
    uint8_t score = 0;

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(position, velocity, playerIndex, aimDirection, score);
    }
};

struct GameSnapshot
{
    std::vector<Player> players;
    std::vector<Bullet> bullets;

    // Needed for Serialize
    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(players, bullets);
    }
};

// https://www.youtube.com/watch?v=S5CXXWbaDlc&list=PL_xRyXins848nDj2v-TJYahzvs-XW9sVV&index=11
class Entity
{
    friend class EntityManager;
private:
    // AvailableComponents are initialized as default, with the "has" member set to false.
    AvailableComponents _components;
    bool _active = true;
    entitytag_t _tag;

    // Entity() {};
    Entity(entitytag_t tag) : _tag(tag) {};
public:
    ~Entity() {};

    entitytag_t GetTag() const
    {
        return _tag;
    }

    template <typename Component>
    Component &GetComponent()
    {
        return std::get<Component>(_components);
    }

    template <typename Component>
    const Component &GetComponent() const
    {
        return std::get<Component>(_components);
    }

    template <typename Component>
    bool HasComponent() const
    {
        return GetComponent<Component>.has;
    }

    template <typename Component, typename... ComponentArgs>
    Component &AddComponent(ComponentArgs&&... args)
    {
        auto &component = GetComponent<Component>();
        component = Component(std::forward<ComponentArgs>(args)...);
        component.has = true;
        return component;
    }
};


#endif /* ENTITIES_HPP */