#include "entity/entity_manager.h"

void EntityManager::update()
{
    for (const Entity& e : m_entitiesToAdd)
    {
        m_entities.emplace_back(e);
    }

    m_entitiesToAdd.clear();
}

Entity EntityManager::addEntity(Lslvs::EntityType type)
{
    Entity e = EntityMemoryPool::instance().addEntity(type);
    m_entitiesToAdd.emplace_back(e);
    return e;
}

std::vector<Entity> EntityManager::getEntities()
{
    return m_entities;
}