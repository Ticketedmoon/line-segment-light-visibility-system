#pragma once

#ifndef LSLVS_SCENE_H
#define LSLVS_SCENE_H

#include <unordered_map>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "action.h"

// Forward-declaration
class GameEngine;

class Scene
{
    public:
        enum class Type
        {
            GAMEPLAY_DEMO_SCENE = 0,
        };
        enum CursorButton
        {
            // These numbers must be outside the keyboard keyCode range to avoid collisions.
            CURSOR_LEFT = 500,
            CURSOR_RIGHT = 501
        };

        explicit Scene(GameEngine& gameEngine);
        virtual ~Scene();

        virtual void update() = 0;
        virtual void render() = 0;
        virtual void performAction(Action& action) = 0;

        void registerActionType(int inputCode, Action::Type type);
        void registerCursorActionType(sf::Event::EventType eventType, Action::Type type);

        [[nodiscard]] bool hasActionType(int inputCode) const;
        bool hasEventType(sf::Event::EventType eventType) const;
        Action::Type getActionType(int inputCode);
        Action::Type getCursorActionType(sf::Event::EventType eventType);

    protected:
        GameEngine& gameEngine;

    private:
        std::unordered_map<int, Action::Type> m_actionMap;
        std::unordered_map<sf::Event::EventType, Action::Type> m_cursorActionMap;
};

#endif //LSLVS_SCENE_H
