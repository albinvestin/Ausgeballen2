#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP
#include "Entities.hpp"
#include "Constants.hpp"
#include <vector>
#include <map>
#include <memory>
// TODO: You should not be #includeing at all. Since you only store pointers, you do not need the full definition of the classes - you only need the declaration. Simply use the forward declarations by themselves

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
private:
    EntityVec _allEntities;
    std::map<entitytag_t, EntityVec> _entityMap;
    EntityVec _entitiesToAddNextUpdate;
public:
    EntityManager() {}
    ~EntityManager() {}

    void Update();
    std::shared_ptr<Entity> AddEntity(entitytag_t tag);
    void RemoveEntity(Entity & entity);
    EntityVec &GetEntities(entitytag_t tag);
    const EntityVec &GetEntities(entitytag_t tag) const;
    EntityVec &GetAllEntities();
    const EntityVec &GetAllEntities() const;
};

#endif /* ENTITYMANAGER_HPP */
