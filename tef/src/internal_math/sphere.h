#pragma once

// Internal
#include "vec3.h"
#include "bounds3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace tef::math
{

    class sphere_t
    {
    public:
        vec3 center;
        f32 radius;

        // Constructors

        constexpr sphere_t()
            : center(vec3(0)), radius(1)
        {}

        constexpr sphere_t(const vec3& center, f32 radius)
            : center(center), radius(radius)
        {}

        // Construct a sphere that bounds a given bounding box
        sphere_t(const bounds3& b);

        // String
        std::string to_string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const sphere_t& r);

        // this == sphere
        constexpr bool operator==(const sphere_t& s) const
        {
            return center == s.center && radius == s.radius;
        }

        // this != sphere
        constexpr bool operator!=(const sphere_t& s) const
        {
            return center != s.center || radius != s.radius;
        }

        // A point on the sphere at given theta and phi angles
        vec3 at(f32 theta, f32 phi) const;

        // Bounding box of the sphere
        constexpr bounds3 bounds() const
        {
            return bounds3(center - radius, center + radius);
        }

    };

    // Check if a point is inside a sphere
    constexpr bool inside(const vec3& p, const sphere_t& s)
    {
        return distance_squared(p, s.center) <= squared(s.radius);
    }

    // Check if two spheres overlap
    constexpr bool overlaps(const sphere_t& s1, const sphere_t& s2)
    {
        return distance_squared(s1.center, s2.center) <= squared(s1.radius + s2.radius);
    }

    // Check if a sphere and a bounding box overlap
    constexpr bool overlaps(const sphere_t& s, const bounds3& b)
    {
        return inside(vec3(b.pmin.x, b.pmin.y, b.pmin.z), s)
            || inside(vec3(b.pmax.x, b.pmin.y, b.pmin.z), s)
            || inside(vec3(b.pmin.x, b.pmax.y, b.pmin.z), s)
            || inside(vec3(b.pmax.x, b.pmax.y, b.pmin.z), s)
            || inside(vec3(b.pmin.x, b.pmin.y, b.pmax.z), s)
            || inside(vec3(b.pmax.x, b.pmin.y, b.pmax.z), s)
            || inside(vec3(b.pmin.x, b.pmax.y, b.pmax.z), s)
            || inside(vec3(b.pmax.x, b.pmax.y, b.pmax.z), s);
    }

    // Check if a sphere and a bounding box overlap
    constexpr bool overlaps(const bounds3& b, const sphere_t& s)
    {
        return overlaps(s, b);
    }

}
