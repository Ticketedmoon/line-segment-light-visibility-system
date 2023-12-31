#include "entity/entity.h"

Entity::Entity(size_t id) : m_id(id)
{
}

size_t Entity::getId() const
{
    return m_id;
}

void Entity::destroy()
{
    EntityMemoryPool::instance().removeEntity(m_id);
}
