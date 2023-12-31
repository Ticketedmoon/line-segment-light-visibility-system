#pragma once

#ifndef LSLVS_COMPONENT_H
#define LSLVS_COMPONENT_H

#include <utility>
#include <SFML/Graphics/VertexArray.hpp>
#include "vec2.h"
#include <SFML/System/Vector3.hpp>
#include "common_constants.h"
#include "vertex.h"
#include "ray.h"

namespace Component
{
    struct CTransform
    {
        std::shared_ptr<Lslvs::Vec2> position;

        bool has{};
    };

    struct CControllable
    {
        bool isMovingUp;
        bool isMovingDown;
        bool isMovingLeft;
        bool isMovingRight;

        bool has;
    };

    struct CCollidable
    {
        bool isCollidingUp{};
        bool isCollidingDown{};
        bool isCollidingLeft{};
        bool isCollidingRight{};

        sf::Vector3f manifoldDist;
        Lslvs::Vec2 collisionOverlap;

        bool has{};
    };

    struct CShape
    {
        sf::VertexArray vertices = sf::VertexArray(sf::LineStrip);

        bool has{};
    };

    struct CLightSource
    {
        // Every 2 elements is a line, so to access the nth line, the index positions are: (v[i * 2], v[(i * 2) * 1])
        std::vector<Lslvs::Ray> rays;

        sf::VertexArray lightVertices;

        std::vector<std::vector<Lslvs::LightRayIntersect>> lightRayIntersects;

        bool has{};
    };
}

#endif //LSLVS_COMPONENT_H
