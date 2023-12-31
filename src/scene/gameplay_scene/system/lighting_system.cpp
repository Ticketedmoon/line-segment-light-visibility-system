#include "lighting_system.h"

LightingSystem::LightingSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LightingSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>() || !entity.hasComponent<Component::CTransform>() ||
                !entity.hasComponent<Component::CShape>())
        {
            continue;
        }

        auto& entityLightSource = entity.getComponent<Component::CLightSource>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        entityLightSource.lightVertices.clear();

        std::vector<Lslvs::LightRayIntersect> intersections = findAllRayIntersectionPoints(entityLightSource,
                entityTransform);

        sortIntersectionsByAngleAscending(entityTransform, intersections);

        // Add to light rendering vertex array
        addVerticesForLightCollisions(entityLightSource, entityTransform, intersections);
    }
}

void LightingSystem::sortIntersectionsByAngleAscending(const Component::CTransform& entityTransform,
        std::vector<Lslvs::LightRayIntersect>& intersections)
{
    auto comparator =
            [&entityTransform](const Lslvs::LightRayIntersect& intersectA, const Lslvs::LightRayIntersect& intersectB) {
                const Lslvs::Vec2 velocityA{intersectA.collisionPoint.x - entityTransform.position->x,
                                            intersectA.collisionPoint.y - entityTransform.position->y};
                const Lslvs::Vec2 velocityB{intersectB.collisionPoint.x - entityTransform.position->x,
                                            intersectB.collisionPoint.y - entityTransform.position->y};
                const float angleA = std::atan2(velocityA.y, velocityA.x);
                const float angleB = std::atan2(velocityB.y, velocityB.x);

                return angleA < angleB;
            };

    std::sort(intersections.begin(), intersections.end(), comparator);
}

std::vector<Lslvs::LightRayIntersect> LightingSystem::findAllRayIntersectionPoints(Component::CLightSource& entityLightSource,
        const Component::CTransform& entityTransform)
{
    std::vector<Lslvs::LightRayIntersect> collisionPoints;
    for (size_t lineIndex = 0; lineIndex <  entityLightSource.rays.size(); lineIndex++)
    {
        std::vector<Lslvs::LightRayIntersect>& intersectList = entityLightSource.lightRayIntersects[lineIndex];
        if (intersectList.empty())
        {
            continue;
        }

        Lslvs::LightRayIntersect closestIntersect = findClosestIntersectForLine(entityTransform, intersectList);
        collisionPoints.emplace_back(closestIntersect);

        intersectList.clear();
    }
    return collisionPoints;
}

void LightingSystem::addVerticesForLightCollisions(Component::CLightSource& entityLightSource,
        const Component::CTransform& entityTransform, const std::vector<Lslvs::LightRayIntersect>& intersections)
{
    // Add transform position of player entity.
    entityLightSource.lightVertices.append({{entityTransform.position->x, entityTransform.position->y}, sf::Color::Yellow});

    // Add all collision points with
    for (const Lslvs::LightRayIntersect& intersection : intersections)
    {
        entityLightSource.lightVertices.append({{intersection.collisionPoint.x, intersection.collisionPoint.y}, sf::Color::Yellow});
    }

    // Add initial collision point again so the TriangleFan can connect forming a full-looking geometric shape displaying visibility.
    entityLightSource.lightVertices.append({{intersections[0].collisionPoint.x, intersections[0].collisionPoint.y}, sf::Color::Yellow});
}

Lslvs::LightRayIntersect LightingSystem::findClosestIntersectForLine(const Component::CTransform& entityTransform,
        std::vector<Lslvs::LightRayIntersect>& intersectList)
{
    Lslvs::LightRayIntersect closestIntersect = intersectList[0];
    double distToPlayer = entityTransform.position->dist(intersectList[0].collisionPoint);
    for (Lslvs::LightRayIntersect intersect: intersectList)
    {
        double nextDistToPlayer = entityTransform.position->dist(intersect.collisionPoint);
        if (nextDistToPlayer < distToPlayer)
        {
            distToPlayer = nextDistToPlayer;
            closestIntersect = intersect;
        }
    }
    return closestIntersect;
}