#include "ray.h"

#include <utility>

namespace Lslvs
{
    Ray::Ray() : m_entityPosition(), m_endVertex(Vec2())
    {

    }
    Ray::~Ray() = default;

    Ray::Ray(std::shared_ptr<Vec2> entityPosition, Vec2 endVertex) : m_entityPosition(std::move(entityPosition)),
                                                                     m_endVertex(endVertex)
    {

    }

    // assignment operator function
    Ray& Ray::operator=(const Ray& other)
    {
        if (this != &other)
        {
            m_entityPosition = other.m_entityPosition;
            m_endVertex = other.m_endVertex;
        }

        return *this;
    }

    std::shared_ptr<Vec2>& Ray::getStartVertex()
    {
        return m_entityPosition;
    }

    Vec2 Ray::getEndVertex() const
    {
        return m_endVertex.position;
    }
}