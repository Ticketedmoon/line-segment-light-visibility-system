#pragma once

#ifndef LSLVS_VEC2_H
#define LSLVS_VEC2_H

#include <cmath>

namespace Lslvs
{
    class Vec2
    {
        public:
            Vec2();
            Vec2(float x, float y);

            bool operator==(const Vec2& rhs) const;
            bool operator!=(const Vec2& rhs) const;

            Vec2 operator+(const Vec2& rhs) const;
            Vec2 operator-(const Vec2& rhs) const;
            Vec2 operator/(float val) const;
            Vec2 operator*(float val) const;

            void operator+=(const Vec2& rhs);
            void operator-=(const Vec2& rhs);
            void operator*=(float val);
            void operator/=(float val);

            [[nodiscard]] double dist(const Vec2& rhs) const;

        public:
            float x{};
            float y{};

    };
}

#endif //LSLVS_VEC2_H
