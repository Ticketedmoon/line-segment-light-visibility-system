#pragma once

#ifndef LSLVS_ENTITY_MEMORY_POOL_H
#define LSLVS_ENTITY_MEMORY_POOL_H

#include "component.h"
#include "common_constants.h"

#include <tuple>
#include <vector>
#include <cassert>
#include <utility>

const inline uint8_t TOTAL_EXPECTED_COMPONENT_TYPES = 5;
const inline uint8_t TOTAL_RESERVED_COMPONENT_TYPE_GROUPS = 5;

typedef std::tuple<
        std::vector<Component::CTransform>,
        std::vector<Component::CControllable>,
        std::vector<Component::CCollidable>,
        std::vector<Component::CShape>,
        std::vector<Component::CLightSource>
> EntityComponentVectorTuple;

static size_t MAX_NUM_ENTITIES = 100;

// forward declare
class Entity;

class EntityMemoryPool
{
    public:
        static EntityMemoryPool& instance()
        {
           static EntityMemoryPool pool(MAX_NUM_ENTITIES);
           return pool;
        }

        [[nodiscard]] Entity addEntity(Lslvs::EntityType type) const;

        void removeEntity(size_t entityId);

    public:
        // Templated functions
        template <typename T>
        std::vector<T>& getComponents()
        {
            return std::get<std::vector<T>>(m_pool);
        }

        template <typename T>
        T& getComponent(const size_t entityId)
        {
            return getComponents<T>()[entityId];
        }

        template <typename T, typename... TArgs>
        T& addComponent(const size_t entityId, TArgs&&... mArgs)
        {
            std::vector<T>& componentPool = std::get<std::vector<T>>(m_pool);
            componentPool[entityId] = T(std::forward<TArgs>(mArgs)...);
            //@review: Is this .has necessary?
            componentPool[entityId].has = true;
            return componentPool[entityId];
        }

        template <typename T>
        bool hasComponent(const size_t entityId)
        {
            std::vector<T>& componentPool = std::get<std::vector<T>>(m_pool);
            return componentPool[entityId].has == true;
        }

        template <typename T>
        void removeComponent(const size_t entityId)
        {
            std::vector<T>& componentPool = std::get<std::vector<T>>(m_pool);
            componentPool[entityId].has = false;
        }

    private:
        explicit EntityMemoryPool(size_t maxNumEntities);

    private:
        static inline size_t m_numEntities = 0;
        static inline EntityComponentVectorTuple m_pool;
        static inline std::vector<Lslvs::EntityType> m_types;
        static inline std::vector<bool> m_alive;
};


#endif //LSLVS_ENTITY_MEMORY_POOL_H
