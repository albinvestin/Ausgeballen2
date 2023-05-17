#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include "Constants.hpp"
#include "Components.hpp"
#include <tuple>

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