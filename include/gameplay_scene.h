#pragma once

#ifndef LSLVS_GAMEPLAY_SCENE_H
#define LSLVS_GAMEPLAY_SCENE_H

#include <SFML/Graphics.hpp>

#include "game_engine.h"
#include "scene.h"

#include "scene/gameplay_scene/entity/entity_spawner.h"

#include "system.h"
#include "render_system.h"
#include "transform_system.h"
#include "collision_system.h"
#include "lighting_system.h"
#include "ray_appender_system.h"

#include "entity_manager.h"
#include "system_manager.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems();
        void registerActions();

    private:
        static const inline sf::Color LEVEL_BACKGROUND_COLOR = sf::Color{0, 32, 32};

        EntityManager m_entityManager;
        EntitySpawner m_entitySpawner;
        SystemManager m_systemManager;

        sf::RenderTexture m_renderTexture;
        sf::Sprite m_renderSprite;
};


#endif //LSLVS_GAMEPLAY_SCENE_H
