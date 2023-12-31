#pragma once

#ifndef LSLVS_LIGHTING_SYSTEM_H
#define LSLVS_LIGHTING_SYSTEM_H

#include <SFML/Graphics/RenderTarget.hpp>
#include "system.h"
#include "entity_manager.h"

class LightingSystem : public System
{
    public:
        explicit LightingSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static std::vector<Lslvs::LightRayIntersect> findAllRayIntersectionPoints(Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform) ;
        static void addVerticesForLightCollisions(Component::CLightSource& entityLightSource,
                const Component::CTransform& entityTransform, const std::vector<Lslvs::LightRayIntersect>& intersections) ;
        static Lslvs::LightRayIntersect findClosestIntersectForLine(const Component::CTransform& entityTransform,
                std::vector<Lslvs::LightRayIntersect>& intersectList) ;
        static void sortIntersectionsByAngleAscending(const Component::CTransform& entityTransform,
                std::vector<Lslvs::LightRayIntersect>& intersections) ;

    private:
        EntityManager& m_entityManager;
};


#endif //LSLVS_LIGHTING_SYSTEM_H
