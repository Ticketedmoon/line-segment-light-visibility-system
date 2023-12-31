#pragma once

#ifndef LSLVS_ENTITY_SPAWNER_H
#define LSLVS_ENTITY_SPAWNER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"
#include "vertex.h"
#include "ray.h"

class EntitySpawner
{
    public:
        explicit EntitySpawner(EntityManager& entityManager);

        void spawnPlayer();
        void spawnWall(Lslvs::Vec2 position, Lslvs::Vec2 dimensions);

    private:
        std::vector<Lslvs::Ray> createRays(Component::CTransform& playerTransform);

    private:
        EntityManager& m_entityManager;
};


#endif //LSLVS_ENTITY_SPAWNER_H
