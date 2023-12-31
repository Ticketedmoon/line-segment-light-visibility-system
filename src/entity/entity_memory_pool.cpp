#include "entity_memory_pool.h"

// Forward declare
#include "entity.h"

EntityMemoryPool::EntityMemoryPool(size_t maxNumEntities)
{
    // ids and alive status
    m_types.reserve(maxNumEntities);
    m_alive.reserve(maxNumEntities);

    // Pool
    auto& transformComponents = std::get<std::vector<Component::CTransform>>(m_pool);
    auto& controllableComponents = std::get<std::vector<Component::CControllable>>(m_pool);
    auto& collisionComponents = std::get<std::vector<Component::CCollidable>>(m_pool);
    auto& rectShapeComponents = std::get<std::vector<Component::CShape>>(m_pool);
    auto& lightSourceComponents = std::get<std::vector<Component::CLightSource>>(m_pool);

    transformComponents.reserve(maxNumEntities);
    controllableComponents.reserve(maxNumEntities);
    collisionComponents.reserve(maxNumEntities);
    rectShapeComponents.reserve(maxNumEntities);
    lightSourceComponents.reserve(maxNumEntities);

    for (size_t i = 0; i < maxNumEntities; i++)
    {
        m_types.insert(m_types.begin() + i, Lslvs::EntityType::NONE);
        m_alive.insert(m_alive.begin() + i, false);

        transformComponents.insert(transformComponents.begin() + i, {{}, false});
        controllableComponents.insert(controllableComponents.begin() + i, {{}, false});
        collisionComponents.insert(collisionComponents.begin() + i, {{}, false});
        rectShapeComponents.insert(rectShapeComponents.begin() + i, {{}, false});
        lightSourceComponents.insert(lightSourceComponents.begin() + i, {{}, {}, {}, false});
    }

    assert(std::tuple_size_v<EntityComponentVectorTuple> == TOTAL_RESERVED_COMPONENT_TYPE_GROUPS);
}

Entity EntityMemoryPool::addEntity(Lslvs::EntityType type) const
{
    assert(std::tuple_size_v<EntityComponentVectorTuple> == TOTAL_EXPECTED_COMPONENT_TYPES);

    // TODO: We should find next entity index, not just increment
    size_t entityId = m_numEntities++;

    Entity entity(entityId);
    m_types[entityId] = type;
    m_alive[entityId] = true;

    return entity;
}

void EntityMemoryPool::removeEntity(size_t entityId)
{
    m_alive[entityId] = false;
}
