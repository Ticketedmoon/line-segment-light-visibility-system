#include "game_engine.h"

GameEngine::GameEngine()
{
    createGameWindow();
    gameScenes[currentScene] = std::make_shared<GameplayScene>(*this);
}

GameEngine::~GameEngine() = default;

void GameEngine::startGameLoop()
{
    double currentTime = gameClock.getElapsedTime().asSeconds();
    double accumulator = 0.0;
    size_t totalFrames = 0;

    while (window.isOpen())
    {
        update(currentTime, accumulator, totalFrames);
        render();

        totalFrames++;
    }
}

// Fixed time-step updates
void GameEngine::update(double& currentTime, double& accumulator, size_t& totalFrames)
{
    double newTime = gameClock.getElapsedTime().asSeconds();
    double frameTime = newTime - currentTime;
    currentTime = newTime;
    accumulator += frameTime;

    while (accumulator >= Lslvs::DT)
    {
        handleInput();
        gameScenes[currentScene]->update();
        accumulator -= Lslvs::DT;
    }

    logDebugInfo(totalFrames, frameTime);
}

void GameEngine::render()
{
    gameScenes[currentScene]->render();
}

void GameEngine::changeScene(Scene::Type sceneType, const std::shared_ptr<Scene>& scene)
{
    currentScene = sceneType;
    gameScenes[currentScene] = scene;
}

// FPS - Shows in Console Window
void GameEngine::logDebugInfo(const size_t& totalFrames, double frameTime) const
{
    if (totalFrames != 0 && totalFrames % 600 == 0)
    {
        std::cout
                << "---------\n"
                << "FPS: " << 1.0f / frameTime << '\n'
                << "Uptime: " << gameClock.getElapsedTime().asSeconds() << " (Seconds)\n"
                << "Total Frames: " << totalFrames << "\n";
    }
}

void GameEngine::handleInput()
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            std::shared_ptr<Scene>& scene = gameScenes[currentScene];
            if (!scene->hasActionType(event.key.code))
            {
                continue;
            }
            const Action::Mode actionMode = event.type == sf::Event::KeyPressed ? Action::Mode::PRESS : Action::Mode::RELEASE;
            Action action = Action(scene->getActionType(event.key.code), actionMode);
            scene->performAction(action);
        }

        if (event.type == sf::Event::MouseEntered || event.type == sf::Event::MouseMoved)
        {
            std::shared_ptr<Scene>& scene = gameScenes[currentScene];
            if (!scene->hasEventType(event.type))
            {
                continue;
            }

            const Action::Mode actionMode = event.type == sf::Event::MouseButtonPressed ? Action::Mode::PRESS : Action::Mode::RELEASE;
            Action action = Action(scene->getCursorActionType(event.type), actionMode);
            scene->performAction(action);
        }

        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
        {
            std::shared_ptr<Scene>& scene = gameScenes[currentScene];
            int buttonCode = event.mouseButton.button == 0 ? Scene::CursorButton::CURSOR_LEFT : Scene::CursorButton::CURSOR_RIGHT;
            if (!scene->hasActionType(buttonCode))
            {
                continue;
            }

            const Action::Mode actionMode = event.type == sf::Event::MouseButtonPressed
                    ? Action::Mode::PRESS
                    : Action::Mode::RELEASE;
            Action action = Action(scene->getActionType(buttonCode), actionMode);
            scene->performAction(action);
        }
    }
}

void GameEngine::createGameWindow()
{
    window.create(sf::VideoMode(Lslvs::WINDOW_WIDTH, Lslvs::WINDOW_HEIGHT), Lslvs::WINDOW_TITLE.data());

    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;
    window.setPosition(sf::Vector2i((screenWidth - Lslvs::WINDOW_WIDTH)/2, (screenHeight - Lslvs::WINDOW_HEIGHT)/2));

    if (Lslvs::USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else
    {
        window.setFramerateLimit(Lslvs::APP_FRAME_RATE);
    }
}
