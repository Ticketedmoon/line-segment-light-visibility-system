#pragma once

#ifndef LSLVS_RAY_APPENDER_SYSTEM_H
#define LSLVS_RAY_APPENDER_SYSTEM_H


#include "system.h"
#include "entity_manager.h"

/**
 * [System Description]
 * This system will add additional rays to the left and right of each vertex to give the visibility effect
 * a more-full look.
*/
class RayAppenderSystem  : public System
{
    public:
        explicit RayAppenderSystem(EntityManager& entityManager);

        void execute() override;

    private:
        EntityManager& m_entityManager;

    private:
        static const int TOTAL_CORE_RAYS = 20;
        static const size_t RAY_DELTA = 1;
        static const size_t RAY_SCALE = 100;
        const size_t totalRaysPerVertex = 4;

};


#endif //LSLVS_RAY_APPENDER_SYSTEM_H
