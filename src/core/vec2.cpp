#include "vec2.h"
namespace Lslvs
{
    Vec2::Vec2() = default;

    Vec2::Vec2(float x, float y) : x(x), y(y)
    {

    }

    bool Vec2::operator==(const Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool Vec2::operator!=(const Vec2& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    Vec2 Vec2::operator+(const Vec2& rhs) const
    {
        return {x + rhs.x, y + rhs.y};
    }

    Vec2 Vec2::operator-(const Vec2& rhs) const
    {
        return {x - rhs.x, y - rhs.y};
    }

    Vec2 Vec2::operator/(float val) const
    {
        return {x - val, y - val};
    }

    Vec2 Vec2::operator*(float val) const
    {
        return {x * val, y * val};
    }

    void Vec2::operator+=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    void Vec2::operator-=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    void Vec2::operator*=(const float val)
    {
        x *= val;
        y *= val;
    }

    void Vec2::operator/=(const float val)
    {
        x /= val;
        y /= val;
    }

    double Vec2::dist(const Vec2& rhs) const
    {
        return sqrt(pow(rhs.x - x, 2) + pow(rhs.y - y, 2) * 1.0);
    }
}