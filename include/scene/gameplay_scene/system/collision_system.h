#pragma once

#ifndef LSLVS_COLLISION_SYSTEM_H
#define LSLVS_COLLISION_SYSTEM_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <SFML/System/Vector3.hpp>

#include "system.h"
#include "entity_manager.h"
#include "common_constants.h"

class CollisionSystem : public System
{
    public:
        explicit CollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static bool isCollidingAABB(const Component::CShape& entityRect,
                const Component::CShape& otherEntityRect, sf::FloatRect& overlap);

        static void resolveCollision(
                const Component::CShape& entityRectangleShape, Component::CTransform& entityTransform,
                const Component::CShape& otherEntityRectangleShape, const Component::CTransform& otherEntityTransform,
                Component::CCollidable& otherEntityCollider,
                const sf::FloatRect& overlap);

        static void resolvePhysicalCollisions(const Component::CShape& entityRectangleShape,
                Component::CTransform& entityTransform, Component::CCollidable& entityCollider,
                const Component::CTransform& otherEntityTransform,
                const Component::CShape& otherEntityRectangleShape);

        static void resolveLightCollisions(const Entity& entity, Component::CShape& otherEntityRectangleShape);

        static void checkForLightIntersectWithShape(Component::CShape& otherEntityShape,
                Component::CLightSource& lightSource, Lslvs::Ray& ray, size_t lineIndex);

        static void checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource, Lslvs::Ray& ray,
                size_t lineIndex, Lslvs::Vec2 windowBorderVertexA, Lslvs::Vec2 windowBorderVertexB);

        static sf::Vector3f getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

        static void applyCollisionManifoldToTransform(Component::CCollidable& entityCollider,
                const sf::FloatRect& overlap, const Lslvs::Vec2& result);

        static float crossProduct(Lslvs::Vec2 a, Lslvs::Vec2 b);

        static Lslvs::LightRayIntersect
        isLineIntersecting(Lslvs::Vec2 vertexA, Lslvs::Vec2 vertexB, Lslvs::Vec2 vertexC,
                Lslvs::Vec2 vertexD);

    private:
        EntityManager& m_entityManager;
};

#endif //LSLVS_COLLISION_SYSTEM_H