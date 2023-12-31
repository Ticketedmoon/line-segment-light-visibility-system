#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Lslvs::EntityType::PLAYER);

    std::shared_ptr<Lslvs::Vec2> position = std::make_shared<Lslvs::Vec2>(Lslvs::WINDOW_WIDTH / 2.0f,
            Lslvs::WINDOW_HEIGHT - 128);
    Lslvs::Vec2 dimensions{Lslvs::PLAYER_SIZE, Lslvs::PLAYER_SIZE};
    
    auto& playerTransform = e.addComponent<Component::CTransform>(position);
    
    std::vector<Lslvs::Ray> rays = createRays(playerTransform);

    sf::VertexArray shapeVertices(sf::Quads);
    const sf::Color playerColor = {255, 0, 127};
    shapeVertices.append(sf::Vertex({position->x - dimensions.x / 2, position->y - dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x + dimensions.x / 2, position->y - dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x + dimensions.x / 2, position->y + dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x - dimensions.x / 2, position->y + dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x - dimensions.x / 2, position->y - dimensions.y / 2}, playerColor));

    std::vector<std::vector<Lslvs::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Lslvs::LightRayIntersect>>(rays.size(), std::vector<Lslvs::LightRayIntersect>());

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CShape>(shapeVertices);
    e.addComponent<Component::CLightSource>(rays, sf::VertexArray(), defaultLightRayIntersects);
}

void EntitySpawner::spawnWall(Lslvs::Vec2 position, Lslvs::Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Lslvs::EntityType::WALL);

    sf::VertexArray vertices(sf::Quads, 5);
    const sf::Color shapeColor = sf::Color::Blue;
    float halfEntityWidth = dimensions.x / 2;
    float halfEntityHeight = dimensions.y / 2;
    vertices[0] = sf::Vertex({position.x - halfEntityWidth, position.y - halfEntityHeight}, shapeColor);
    vertices[1] = sf::Vertex({position.x + halfEntityWidth, position.y - halfEntityHeight}, shapeColor);
    vertices[2] = sf::Vertex({position.x + halfEntityWidth, position.y + halfEntityHeight}, shapeColor);
    vertices[3] = sf::Vertex({position.x - halfEntityWidth, position.y + halfEntityHeight}, shapeColor);
    vertices[4] = sf::Vertex({position.x - halfEntityWidth, position.y - halfEntityHeight}, shapeColor);

    e.addComponent<Component::CTransform>(std::make_shared<Lslvs::Vec2>(position));
    e.addComponent<Component::CShape>(vertices);
}

std::vector<Lslvs::Ray> EntitySpawner::createRays(Component::CTransform& playerTransform)
{
    std::vector<Lslvs::Ray> rays = std::vector<Lslvs::Ray>();
    std::vector<Lslvs::Ray> additionalRays = std::vector<Lslvs::Ray>();

    // Add window corner vertex rays
    rays.emplace_back(playerTransform.position, Lslvs::Vec2(0, 0));
    rays.emplace_back(playerTransform.position, Lslvs::Vec2(Lslvs::WINDOW_WIDTH, 0));
    rays.emplace_back(playerTransform.position, Lslvs::Vec2(0, Lslvs::WINDOW_HEIGHT));
    rays.emplace_back(playerTransform.position, Lslvs::Vec2(Lslvs::WINDOW_WIDTH, Lslvs::WINDOW_HEIGHT));

    // Add wall vertices rays
    std::vector<Entity> wallEntities = m_entityManager.getEntitiesByComponentType<Component::CShape>();
    for (auto wallEntity : wallEntities)
    {
        Component::CShape s = wallEntity.getComponent<Component::CShape>();
        for (size_t vertIndex = 0; vertIndex < s.vertices.getVertexCount()-1; vertIndex++)
        {
            sf::Vertex v = s.vertices[vertIndex];

            // Add core ray
            rays.emplace_back(playerTransform.position, Lslvs::Vec2(v.position.x, v.position.y));

            // Add additional rays to left and right of core ray (This happens in RayAppenderSystem)
            additionalRays.emplace_back(playerTransform.position, Lslvs::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Lslvs::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Lslvs::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Lslvs::Vec2(0, 0));
        }
    }

    rays.insert(rays.end(), additionalRays.begin(), additionalRays.end());

    std::cout << "Configured: [" << rays.size() << "] light rays" << '\n';

    return rays;
}