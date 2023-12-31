#include "collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    for (const Entity entity : entities)
    {
        auto& entityRectangleShape = entity.getComponent<Component::CShape>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        auto& entityCollider = entity.getComponent<Component::CCollidable>();

        for (const Entity otherEntity : entities)
        {
            if (entity.getId() == otherEntity.getId())
            {
                // @Refactor: Can we do this a better way?
                continue;
            }

            auto& otherEntityRectangleShape = otherEntity.getComponent<Component::CShape>();
            auto& otherEntityTransform = otherEntity.getComponent<Component::CTransform>();
            resolvePhysicalCollisions(entityRectangleShape, entityTransform, entityCollider, otherEntityTransform, otherEntityRectangleShape);
            resolveLightCollisions(entity, otherEntityRectangleShape);
        }
    }
}

void CollisionSystem::resolveLightCollisions(const Entity& entity, Component::CShape& otherEntityRectangleShape)
{
    if (!entity.hasComponent<Component::CLightSource>())
    {
        return;
    }

    auto& lightSource = entity.getComponent<Component::CLightSource>();
    for (size_t rayIndex = 0; rayIndex < lightSource.rays.size(); rayIndex++)
    {
        // @Refactor: If ray collision with shape, we don't need to check window border collisions.
        Lslvs::Ray& ray = lightSource.rays[rayIndex];
        checkForLightIntersectWithShape(otherEntityRectangleShape, lightSource, ray, rayIndex);

        // top
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex, Lslvs::Vec2(0, 0),
                Lslvs::Vec2(Lslvs::WINDOW_WIDTH, 0));
        // right
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex,
                Lslvs::Vec2(Lslvs::WINDOW_WIDTH, 0),
                Lslvs::Vec2(Lslvs::WINDOW_WIDTH, Lslvs::WINDOW_HEIGHT));
        // bottom
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex,
                Lslvs::Vec2(0, Lslvs::WINDOW_HEIGHT),
                Lslvs::Vec2(Lslvs::WINDOW_WIDTH, Lslvs::WINDOW_HEIGHT));
        // left
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex, Lslvs::Vec2(0, 0),
                Lslvs::Vec2(0, Lslvs::WINDOW_HEIGHT));
    }
}

void CollisionSystem::resolvePhysicalCollisions(const Component::CShape& entityRectangleShape,
        Component::CTransform& entityTransform, Component::CCollidable& entityCollider,
        const Component::CTransform& otherEntityTransform,
        const Component::CShape& otherEntityRectangleShape)
{
    sf::FloatRect overlap;
    if (isCollidingAABB(entityRectangleShape, otherEntityRectangleShape, overlap))
    {
        resolveCollision(entityRectangleShape, entityTransform, otherEntityRectangleShape, otherEntityTransform,
                entityCollider, overlap);
    }
}

void CollisionSystem::checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource, Lslvs::Ray& ray,
        size_t lineIndex, Lslvs::Vec2 windowBorderVertexA, Lslvs::Vec2 windowBorderVertexB)
{
    Lslvs::Vec2 rayStartPos = {ray.getStartVertex()->x, ray.getStartVertex()->y};
    Lslvs::Vec2 rayEndPos = {ray.getEndVertex().x, ray.getEndVertex().y};
    Lslvs::LightRayIntersect windowBorderIntersection = isLineIntersecting(rayStartPos,
            rayEndPos, windowBorderVertexA, windowBorderVertexB);
    if (windowBorderIntersection.hasIntersection)
    {
        lightSource.lightRayIntersects[lineIndex].emplace_back(windowBorderIntersection);
    }
}

void CollisionSystem::checkForLightIntersectWithShape(Component::CShape& otherEntityShape,
        Component::CLightSource& lightSource, Lslvs::Ray& ray, size_t lineIndex)
{
    // @Refactor: Rather than order these in reverse, sort by closest distance to line for a more scalable solution.
    // [0, 1] = top, [1, 2] = right, [2, 3] = bottom, [3, 4] = left
    for (size_t shapeSideIndex = 0; shapeSideIndex < otherEntityShape.vertices.getVertexCount()-1; shapeSideIndex++)
    {
        Lslvs::Vec2 rayStartPos = {ray.getStartVertex()->x, ray.getStartVertex()->y};
        Lslvs::Vec2 rayEndPos = {ray.getEndVertex().x, ray.getEndVertex().y};

        sf::Vertex& otherShapeStartVert = otherEntityShape.vertices[shapeSideIndex];
        sf::Vertex& otherShapeEndVert = otherEntityShape.vertices[shapeSideIndex + 1];

        Lslvs::Vec2 shapeLineStartPos{otherShapeStartVert.position.x, otherShapeStartVert.position.y};
        Lslvs::Vec2 shapeLineEndPos{otherShapeEndVert.position.x, otherShapeEndVert.position.y};

        Lslvs::LightRayIntersect shapeLightRayIntersection = isLineIntersecting(rayStartPos,
                rayEndPos, shapeLineStartPos, shapeLineEndPos);

        if (!shapeLightRayIntersection.hasIntersection)
        {
            continue;
        }

        lightSource.lightRayIntersects[lineIndex].emplace_back(shapeLightRayIntersection);
    }
}

bool CollisionSystem::isCollidingAABB(const Component::CShape& entityRect,
        const Component::CShape& otherEntityRect, sf::FloatRect& overlap)
{
    return entityRect.vertices.getBounds().intersects(otherEntityRect.vertices.getBounds(), overlap);
}

Lslvs::LightRayIntersect CollisionSystem::isLineIntersecting(Lslvs::Vec2 vertexA,
        Lslvs::Vec2 vertexB, Lslvs::Vec2 vertexC, Lslvs::Vec2 vertexD)
{
    Lslvs::Vec2 r = (vertexB - vertexA);
    Lslvs::Vec2 s = (vertexD - vertexC);
    Lslvs::Vec2 cma = vertexC - vertexA;

    float rCrossS = crossProduct(r, s);
    float cmaCrossS = crossProduct(cma, s);
    float cmaCrossR = crossProduct(cma, r);

    float t = (cmaCrossS / rCrossS);
    float u = (cmaCrossR / rCrossS);

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        float collisionVertexX = vertexA.x + (t * r.x);
        float collisionVertexY = vertexA.y + (t * r.y);
        return {true, Lslvs::Vec2(collisionVertexX, collisionVertexY)};
    }

    return {false, Lslvs::Vec2(0, 0)};
}

float CollisionSystem::crossProduct(Lslvs::Vec2 a, Lslvs::Vec2 b)
{
    return a.x * b.y - b.x * a.y;
}

void CollisionSystem::resolveCollision(
        const Component::CShape& entityRectangleShape, Component::CTransform& entityTransform,
        const Component::CShape& otherEntityRectangleShape, const Component::CTransform& otherEntityTransform,
        Component::CCollidable& otherEntityCollider,
        const sf::FloatRect& overlap)
{
    float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
    float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

    float oxDiff = std::abs(otherEntityRectangleShape.vertices[0].position.x - otherEntityRectangleShape.vertices[1].position.x) / 2;
    float oyDiff = std::abs(otherEntityRectangleShape.vertices[0].position.y - otherEntityRectangleShape.vertices[2].position.y) / 2;

    const Lslvs::Vec2& entityPosition = Lslvs::Vec2(entityTransform.position->x - xDiff, entityTransform.position->y + yDiff);
    const Lslvs::Vec2& otherEntityPosition = Lslvs::Vec2(otherEntityTransform.position->x - oxDiff, otherEntityTransform.position->y + oyDiff);
    const Lslvs::Vec2& result = entityPosition - otherEntityPosition;

    applyCollisionManifoldToTransform(otherEntityCollider, overlap, result);
}

void CollisionSystem::applyCollisionManifoldToTransform(Component::CCollidable& entityCollider,
        const sf::FloatRect& overlap, const Lslvs::Vec2& result)
{
    sf::Vector2f collisionNormal{result.x, result.y};
    sf::Vector3f manifold = getManifold(overlap, collisionNormal);

    entityCollider.manifoldDist = manifold;
    entityCollider.collisionOverlap = {overlap.width, overlap.height};
}

sf::Vector3f CollisionSystem::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
    // the collision normal is stored in x and y, with the penetration in z
    sf::Vector3f manifold;

    if (overlap.width < overlap.height)
    {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    }
    else
    {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}