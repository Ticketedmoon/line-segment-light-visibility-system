#pragma once

#ifndef LSLVS_RAY_H
#define LSLVS_RAY_H

#include <SFML/Graphics/Vertex.hpp>
#include "vertex.h"
#include <iostream>
#include <memory>

// TODO @investigate: Should ray contain collision information? e.g., collision point, corner points of collision side
namespace Lslvs
{
    class Ray
    {
        public:
            Ray();
            ~Ray();
            explicit Ray(std::shared_ptr<Vec2> entityPosition, Vec2 endVertex);

            // assignment operator overload.
            Ray& operator=(const Ray& other);

            [[nodiscard]] std::shared_ptr<Vec2>& getStartVertex();
            [[nodiscard]] Vec2 getEndVertex() const;

        private:
            std::shared_ptr<Vec2> m_entityPosition;
            Lslvs::Vertex m_endVertex;
    };
}

#endif //LSLVS_RAY_H
