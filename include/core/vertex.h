#pragma once

#ifndef LSLVS_VERTEX_H
#define LSLVS_VERTEX_H

#include <SFML/Graphics/Color.hpp>
#include "vec2.h"

namespace Lslvs
{
    class Vertex
    {
        public:
            Vec2 position;
            sf::Color colour;
    };
}


#endif //LSLVS_VERTEX_H
