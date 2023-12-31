#pragma once

#ifndef LSLVS_ENTITY_MANAGER_H
#define LSLVS_ENTITY_MANAGER_H


#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include "entity.h"

class EntityManager
{
    public:
        void update();
        Entity addEntity(Lslvs::EntityType type);
        std::vector<Entity> getEntities();

        template<typename T>
        std::vector<Entity> getEntitiesByComponentType()
        {
            std::ranges::filter_view filteredEntities = m_entities |
                    std::ranges::views::filter([](Entity& e) {
                        return e.hasComponent<T>();
                    });
            return std::vector(filteredEntities.begin(), filteredEntities.end());
        };

        // TODO is this needed?
        template<typename T>
        std::vector<T> getActiveComponentGroupForEntities()
        {
            std::vector<T> activeComponents;
            std::vector<T> componentGroup = EntityMemoryPool::instance().getComponents<T>();
            for (T component : componentGroup)
            {
                if (component.has)
                {
                    activeComponents.emplace_back(component);
                }
            }
            return activeComponents;
        }

    private:
        std::vector<Entity> m_entities;
        std::vector<Entity> m_entitiesToAdd; // Holds enemies to add each frame, will be added to m_entities on next frame update
        std::unordered_map<Lslvs::EntityType, std::vector<Entity>> m_entitiesByType;

};


#endif //LSLVS_ENTITY_MANAGER_H
