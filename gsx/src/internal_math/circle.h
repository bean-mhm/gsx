#pragma once

#include "vec2.h"
#include "bounds2.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    class circle_t
    {
    public:
        vec2 center;
        f32 radius;

        constexpr circle_t()
            : center(vec2(0)), radius(1)
        {}

        constexpr circle_t(const vec2& center, f32 radius)
            : center(center), radius(radius)
        {}

        // construct a circle that bounds a given bounding box
        circle_t(const bounds2& b);

        std::string to_string() const;

        friend std::ostream& operator<<(std::ostream& os, const circle_t& c);

        constexpr bool operator==(const circle_t& c) const
        {
            return center == c.center && radius == c.radius;
        }

        constexpr bool operator!=(const circle_t& c) const
        {
            return center != c.center || radius != c.radius;
        }

        constexpr bounds2 bounds() const
        {
            return bounds2(center - radius, center + radius);
        }

        // a point on the circle at a given angle
        vec2 at(f32 angle) const;

        // a point on the unit circle at a given angle
        static vec2 unit_at(f32 angle);

    };

    inline bool inside(const vec2& p, const circle_t& c)
    {
        return distance_squared(p, c.center) <= squared(c.radius);
    }

    inline bool overlaps(const circle_t& c1, const circle_t& c2)
    {
        return
            distance_squared(c1.center, c2.center)
            <= squared(c1.radius + c2.radius);
    }

    inline bool overlaps(const circle_t& c, const bounds2& b)
    {
        return inside(b.pmin, c)
            || inside(vec2(b.pmax.x, b.pmin.y), c)
            || inside(vec2(b.pmin.x, b.pmax.y), c)
            || inside(b.pmax, c);
    }

    inline bool overlaps(const bounds2& b, const circle_t& c)
    {
        return overlaps(c, b);
    }

}
