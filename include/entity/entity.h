#pragma once

#ifndef LSLVS_ENTITY_H
#define LSLVS_ENTITY_H

#include <ranges>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "component.h"
#include "entity_memory_pool.h"

class Entity
{
    public:
        friend class EntityManager;

    public:
        explicit Entity(size_t id);
        [[nodiscard]] size_t getId() const;
        void destroy();
        
    public:
        template <typename T>
        T& getComponent() const
        {
            return EntityMemoryPool::instance().getComponent<T>(m_id);
        }

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)
        {
            return EntityMemoryPool::instance().addComponent<T>(m_id, mArgs...);
        }

        template<typename T>
        [[nodiscard]] bool hasComponent() const
        {
            return EntityMemoryPool::instance().hasComponent<T>(m_id);
        }

        template<typename T>
        bool hasComponents(std::vector<T> components) const
        {
            for (T component: components)
            {
                if (!EntityMemoryPool::instance().hasComponent<T>(m_id))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename T>
        void removeComponent()
        {
            EntityMemoryPool::instance().removeComponent<T>(m_id);
        }

    private:
        size_t m_id;

};


#endif //LSLVS_ENTITY_H
