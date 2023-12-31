#include "gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine),
    m_entitySpawner(m_entityManager)
{
    m_renderTexture.create(Lslvs::WINDOW_WIDTH, Lslvs::WINDOW_HEIGHT);
    m_renderSprite.setTexture(m_renderTexture.getTexture());
    m_renderSprite.setTextureRect(sf::IntRect(0, 0, Lslvs::WINDOW_WIDTH, Lslvs::WINDOW_HEIGHT));

    registerActions();
    registerSystems();

    // TODO move this to be config driven
    m_entitySpawner.spawnWall({150, 150}, {100, 100});
    m_entitySpawner.spawnWall({Lslvs::WINDOW_WIDTH-150, 150}, {100, 100});
    m_entitySpawner.spawnWall({150, Lslvs::WINDOW_HEIGHT-150}, {100, 100});
    m_entitySpawner.spawnWall({Lslvs::WINDOW_WIDTH-150, Lslvs::WINDOW_HEIGHT-150}, {100, 100});
    m_entitySpawner.spawnWall({Lslvs::WINDOW_WIDTH/2, Lslvs::WINDOW_HEIGHT/2}, {200, 200});
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update();

    if (m_entityManager.getEntitiesByComponentType<Component::CControllable>().empty())
    {
        m_entitySpawner.spawnPlayer();
    }
}

void GameplayScene::render()
{
    m_renderTexture.clear(LEVEL_BACKGROUND_COLOR);
    m_systemManager.render();
    m_renderTexture.display();
    m_renderSprite.setTexture(m_renderTexture.getTexture());

    gameEngine.window.clear();
    gameEngine.window.draw(m_renderSprite, sf::RenderStates(sf::BlendAdd));
    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    if (Action::Type::SCENE_EXIT == action.getType())
    {
        if (action.getMode() == Action::Mode::RELEASE)
        {
            return;
        }
        gameEngine.window.close();
    }

    std::vector<Entity> controllableEntities = m_entityManager.getEntitiesByComponentType<Component::CControllable>();
    for (const Entity& e : controllableEntities)
    {
        auto& cControllable = e.getComponent<Component::CControllable>();
        if (Action::Type::MOVE_LEFT == action.getType())
        {
            cControllable.isMovingLeft = action.getMode() == Action::Mode::PRESS;
        }

        if (Action::Type::MOVE_RIGHT == action.getType())
        {
            cControllable.isMovingRight = action.getMode() == Action::Mode::PRESS;
        }

        if (Action::Type::MOVE_UP == action.getType())
        {
            cControllable.isMovingUp = action.getMode() == Action::Mode::PRESS;
        }

        if (Action::Type::MOVE_DOWN == action.getType())
        {
            cControllable.isMovingDown = action.getMode() == Action::Mode::PRESS;
        }
    }
}

void GameplayScene::registerActions()
{
    // Escape
    registerActionType(sf::Keyboard::Key::Escape, Action::Type::SCENE_EXIT);

    // Movement
    registerActionType(sf::Keyboard::Key::Left, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Key::Right, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Key::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::Down, Action::Type::MOVE_DOWN);

    registerActionType(sf::Keyboard::Key::A, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Key::D, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Key::W, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::S, Action::Type::MOVE_DOWN);
}

void GameplayScene::registerSystems()
{
    m_systemManager.registerSystem(
            std::make_shared<TransformSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    // FIXME: When pushing blocks, there is a flickering effect still, as the ray append system comes before the
    //        collision system. Flipping the order of these systems still causes issues, we need to think about
    //        combining the systems in some way, such that the pushing transformation is done prior to the RayAppenderSystem.
    //        However, it also much be before the light collisions are calculated in the CollisionSystem.
    m_systemManager.registerSystem(
            std::make_shared<RayAppenderSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<CollisionSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(
            std::make_shared<LightingSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(
            std::make_shared<RenderSystem>(m_renderTexture, m_entityManager), SystemManager::SystemType::RENDER);
}