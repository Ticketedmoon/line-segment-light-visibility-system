#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void TransformSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity& entity : entities)
    {
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        if (Lslvs::WORLD_ENTITIES_COLLIDABLE)
        {
            auto& entityCollider = entity.getComponent<Component::CCollidable>();
            applyCollisionOverlapToEntityTransform(entityTransform, entityCollider);
        }

        // @Refactor: Should we do this elsewhere?
        if (entity.hasComponent<Component::CControllable>())
        {
            resolveControllerMovementForEntity(entity, entityTransform);
        }

        auto& entityRectangleShape = entity.getComponent<Component::CShape>();
        applyMovementToEntityTransformFromInput(entityTransform, entityRectangleShape);
    }
}

void TransformSystem::applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
        Component::CCollidable& entityCollider)
{
    if (entityCollider.manifoldDist.y == 1)
    {
        // Bottom Collision
        entityTransform.position->y -= entityCollider.collisionOverlap.y;
    }
    if (entityCollider.manifoldDist.y == -1)
    {
        // Top Collision
        entityTransform.position->y += entityCollider.collisionOverlap.y;
    }

    if (entityCollider.manifoldDist.x == 1)
    {
        // Left Collision
        entityTransform.position->x -= entityCollider.collisionOverlap.x;
    }
    if (entityCollider.manifoldDist.x == -1)
    {
        // Right Collision
        entityTransform.position->x += entityCollider.collisionOverlap.x;
    }

    entityCollider.manifoldDist = {0, 0, 0};
    entityCollider.collisionOverlap = {0, 0};
}

void TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform)
{
    auto& controllable = e.getComponent<Component::CControllable>();
    float SPEED = 3.5f;

    if (controllable.isMovingLeft)
    {
        cTransform.position->x -= SPEED;
    }
    if (controllable.isMovingRight)
    {
        cTransform.position->x += SPEED;
    }
    if (controllable.isMovingUp)
    {
        cTransform.position->y -= SPEED;
    }
    if (controllable.isMovingDown)
    {
        cTransform.position->y += SPEED;
    }
}

void TransformSystem::applyMovementToEntityTransformFromInput(const Component::CTransform& entityTransform,
        Component::CShape& entityRectangleShape)
{
    float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
    float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

    // Update rect based on transform points
    entityRectangleShape.vertices[0].position = { entityTransform.position->x - xDiff, entityTransform.position->y - yDiff };
    entityRectangleShape.vertices[1].position = { entityTransform.position->x + xDiff, entityTransform.position->y - yDiff };
    entityRectangleShape.vertices[2].position = { entityTransform.position->x + xDiff, entityTransform.position->y + yDiff };
    entityRectangleShape.vertices[3].position = { entityTransform.position->x - xDiff, entityTransform.position->y + yDiff };
    entityRectangleShape.vertices[4].position = { entityTransform.position->x - xDiff, entityTransform.position->y - yDiff };
}
