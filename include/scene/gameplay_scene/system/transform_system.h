#pragma once

#ifndef LSLVS_TRANSFORM_SYSTEM_H
#define LSLVS_TRANSFORM_SYSTEM_H

#include <cmath>
#include <cassert>
#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "system.h"
#include "entity/entity_manager.h"
#include "scene/gameplay_scene/system/render_system.h"

class TransformSystem : public System
{
    public:
        explicit TransformSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform);
        static void applyMovementToEntityTransformFromInput(const Component::CTransform& entityTransform,
                Component::CShape& entityRectangleShape);
        static void applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
                Component::CCollidable& entityCollider);

    private:
        EntityManager& m_entityManager;
};


#endif //LSLVS_TRANSFORM_SYSTEM_H
