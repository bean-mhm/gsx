#pragma once

// Internal
#include "vec2.h"
#include "bounds2.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace tef::math
{

    class circle_t
    {
    public:
        vec2 center;
        f32 radius;

        // Constructors

        constexpr circle_t()
            : center(vec2(0)), radius(1)
        {}

        constexpr circle_t(const vec2& center, f32 radius)
            : center(center), radius(radius)
        {}

        // Construct a circle that bounds a given bounding box
        circle_t(const bounds2& b);

        // String
        std::string to_string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const circle_t& r);

        // this == circle
        constexpr bool operator==(const circle_t& c) const
        {
            return center == c.center && radius == c.radius;
        }

        // this != circle
        constexpr bool operator!=(const circle_t& c) const
        {
            return center != c.center || radius != c.radius;
        }

        // A point on the circle at a given angle
        vec2 at(f32 angle) const;

        // Bounding box of the circle
        constexpr bounds2 bounds() const
        {
            return bounds2(center - radius, center + radius);
        }

    };

    // Check if a point is inside a circle
    constexpr bool inside(const vec2& p, const circle_t& c)
    {
        return distance_squared(p, c.center) <= squared(c.radius);
    }

    // Check if two circles overlap
    constexpr bool overlaps(const circle_t& c1, const circle_t& c2)
    {
        return distance_squared(c1.center, c2.center) <= squared(c1.radius + c2.radius);
    }

    // Check if a circle and a bounding box overlap
    constexpr bool overlaps(const circle_t& c, const bounds2& b)
    {
        return inside(b.pmin, c)
            || inside(vec2(b.pmax.x, b.pmin.y), c)
            || inside(vec2(b.pmin.x, b.pmax.y), c)
            || inside(b.pmax, c);
    }

    // Check if a circle and a bounding box overlap
    constexpr bool overlaps(const bounds2& b, const circle_t& c)
    {
        return overlaps(c, b);
    }

}
