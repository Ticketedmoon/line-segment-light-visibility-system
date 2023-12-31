#pragma once

#ifndef LSLVS_COMMON_CONSTANTS_H
#define LSLVS_COMMON_CONSTANTS_H

#include <cstdint>
#include <string_view>
#include "vec2.h"

namespace Lslvs
{
    static constexpr std::string_view WINDOW_TITLE = "line-segment-light-visibility-system";
    static constexpr bool USE_VERTICAL_SYNC = true;
    static constexpr uint32_t APP_FRAME_RATE = 60;
    static constexpr uint32_t WINDOW_WIDTH = 1280; // 20 tiles
    static constexpr uint32_t WINDOW_HEIGHT = 768; // 12 tiles
    static constexpr float DT = 0.01;

    static constexpr float PLAYER_SIZE = 50;

    static constexpr bool WORLD_ENTITIES_COLLIDABLE = false;

    enum class EntityType
    {
        PLAYER,
        WALL,
        NONE
    };

    struct LightRayIntersect
    {
        bool hasIntersection{};
        Vec2 collisionPoint;
    };
}

#endif //LSLVS_COMMON_CONSTANTS_H
