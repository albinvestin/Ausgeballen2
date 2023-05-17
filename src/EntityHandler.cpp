#include "../inc/EntityHandler.hpp"
//#include "../inc/Player.hpp"
#include "../inc/Entities.hpp"
#include "../inc/Constants.hpp"
#include <cmath>
#include <stdio.h>
#include <algorithm>

std::shared_ptr<Entity> EntityManager::AddEntity(entitytag_t tag)
{
    auto newEntity = std::shared_ptr<Entity>(new Entity(tag));
    _entitiesToAddNextUpdate.push_back(newEntity);
    return newEntity;
}

// Removes the entity on the next update.
void EntityManager::RemoveEntity(Entity & entity)
{
    entity._active = false;
}

EntityVec &EntityManager::GetEntities(entitytag_t tag)
{
    const auto &vec = _entityMap.find(tag);
    if (vec != _entityMap.end())
    {
        return vec->second;
    }
    else
    {
        static std::vector<std::shared_ptr<Entity>> emptyVec{};
        return emptyVec;
    }
}
const EntityVec &EntityManager::GetEntities(entitytag_t tag) const
{
    const auto &vec = _entityMap.find(tag);
    if (vec != _entityMap.end())
    {
        return vec->second;
    }
    else
    {
        static std::vector<std::shared_ptr<Entity>> emptyVec{};
        return emptyVec;
    }
}

EntityVec &EntityManager::GetAllEntities()
{
    return _allEntities;
}
const EntityVec &EntityManager::GetAllEntities() const
{
    return _allEntities;
}

void EntityManager::Update()
{
    // Remove inactive entities, first remove the reference, then the entity.
    auto iteratorOverTags = _entityMap.begin();
    while (iteratorOverTags != _entityMap.end())
    {
        auto &entities = iteratorOverTags->second;
        entities.erase(std::remove_if(entities.begin(), entities.end(), 
            [](const std::shared_ptr<Entity> &e) {return !e->_active;}), entities.end());
        // Never remove tags with no entities, just keep them laying.
        iteratorOverTags++;
    }
    _allEntities.erase(std::remove_if(_allEntities.begin(), _allEntities.end(), 
        [](const std::shared_ptr<Entity> &e) {return !e->_active;}), _allEntities.end());

    // Add new entites
    for (auto &&entity : _entitiesToAddNextUpdate)
    {
        _allEntities.push_back(entity);
        _entityMap[entity->_tag].push_back(entity);
    }
    _entitiesToAddNextUpdate.clear();
}
