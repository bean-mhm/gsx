#pragma once

// Internal
#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"

namespace tef::math
{

    template<typename T>
    class base_bounds3
    {
    public:
        base_vec3<T> pmin, pmax;

        // Constructors

        constexpr base_bounds3()
            : pmin(base_vec3<T>(std::numeric_limits<T>::max())),
            pmax(base_vec3<T>(std::numeric_limits<T>::lowest()))
        {}

        constexpr base_bounds3(const base_vec3<T>& p)
            : pmin(p), pmax(p)
        {}

        constexpr base_bounds3(const base_vec3<T>& p1, const base_vec3<T>& p2)
            : pmin(min(p1, p2)), pmax(max(p1, p2))
        {}

        // Type casting
        template<typename U>
        constexpr operator base_bounds3<U>() const
        {
            return base_bounds3<U>((base_vec3<U>)pmin, (base_vec3<U>)pmax);
        }

        // String
        std::string to_string() const
        {
            return std::format(
                "[pmin={}, pmax={}]",
                pmin.to_string(),
                pmax.to_string()
            );
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const base_bounds3<T>& b)
        {
            os << b.to_string();
            return os;
        }

        // Access by index (const reference)
        constexpr const base_vec3<T>& operator[](i32 i) const
        {
            if (i == 0) return pmin;
            return pmax;
        }

        // Access by index (reference)
        constexpr base_vec3<T>& operator[](i32 i)
        {
            if (i == 0) return pmin;
            return pmax;
        }

        // this == bounds
        constexpr bool operator==(const base_bounds3<T>& b) const
        {
            return pmin == b.pmin && pmax == b.pmax;
        }

        // this != bounds
        constexpr bool operator!=(const base_bounds3<T>& b) const
        {
            return pmin != b.pmin || pmax != b.pmax;
        }

        // The coordinates of one of the eight corners of the bounding box
        constexpr base_vec3<T> corner(i32 i) const
        {
            return base_vec3<T>(
                (*this)[(i & 1)].x,
                (*this)[(i & 2) ? 1 : 0].y,
                (*this)[(i & 4) ? 1 : 0].z
            );
        }

        // The vector along the box diagonal from the minimum point to the maximum point
        constexpr base_vec3<T> diagonal() const
        {
            return pmax - pmin;
        }

        // The surface area of the six faces of the box
        constexpr T surface_area() const
        {
            base_vec3<T> d = diagonal();
            return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
        }

        // The volume of the bounding box
        constexpr T volume() const
        {
            base_vec3<T> d = diagonal();
            return d.x * d.y * d.z;
        }

        // The index of which of the axes is longest
        constexpr i32 max_extent() const
        {
            base_vec3<T> d = diagonal();
            if (d.x > d.y && d.x > d.z)
                return 0;
            else if (d.y > d.z)
                return 1;
            else
                return 2;
        }

        // Linear interpolation between the corners of the box by the given amount in each
        // dimension
        constexpr base_vec3<T> lerp(const base_vec3<T>& t) const
        {
            return base_vec3<T>(
                mix(pmin.x, pmax.x, t.x),
                mix(pmin.y, pmax.y, t.y),
                mix(pmin.z, pmax.z, t.z)
            );
        }

        // The continuous position of a point relative to the corners of the box, where a point at
        // the minimum corner has offset (0, 0, 0), a point at the maximum corner has offset
        // (1, 1, 1), and so forth.
        constexpr base_vec3<T> offset_of(const base_vec3<T>& p) const
        {
            base_vec3<T> o = p - pmin;
            if (pmax.x > pmin.x) o.x /= pmax.x - pmin.x;
            if (pmax.y > pmin.y) o.y /= pmax.y - pmin.y;
            if (pmax.z > pmin.z) o.z /= pmax.z - pmin.z;
            return o;
        }

    };

    // Bounds + point
    template<typename T>
    constexpr base_bounds3<T> union_(const base_bounds3<T>& b, const base_vec3<T>& p)
    {
        return base_bounds3<T>(min(b.pmin, p), max(b.pmax, p));
    }

    // Bounds + bounds
    template<typename T>
    constexpr base_bounds3<T> union_(const base_bounds3<T>& b1, const base_bounds3<T>& b2)
    {
        return base_bounds3<T>(min(b1.pmin, b2.pmin), max(b1.pmax, b2.pmax));
    }

    // Bounds * bounds
    template<typename T>
    constexpr base_bounds3<T> intersect(const base_bounds3<T>& b1, const base_bounds3<T>& b2)
    {
        return base_bounds3<T>(max(b1.pmin, b2.pmin), min(b1.pmax, b2.pmax));
    }

    // Check if two bounding boxes overlap
    template<typename T>
    constexpr bool overlaps(const base_bounds3<T>& b1, const base_bounds3<T>& b2)
    {
        return b1.pmax.x >= b2.pmin.x && b1.pmin.x <= b2.pmax.x
            && b1.pmax.y >= b2.pmin.y && b1.pmin.y <= b2.pmax.y
            && b1.pmax.z >= b2.pmin.z && b1.pmin.z <= b2.pmax.z;
    }

    // Check if a point is inside a bounding box
    template<typename T>
    constexpr bool inside(const base_vec3<T>& p, const base_bounds3<T>& b)
    {
        return p.x >= b.pmin.x && p.x <= b.pmax.x
            && p.y >= b.pmin.y && p.y <= b.pmax.y
            && p.z >= b.pmin.z && p.z <= b.pmax.z;
    }

    // The inside_exclusive() variant of inside() doesn't consider points on the upper boundary to
    // be inside the bounds. It is mostly useful with integer-typed bounds.
    template<typename T>
    constexpr bool inside_exclusive(const base_vec3<T>& p, const base_bounds3<T>& b)
    {
        return p.x >= b.pmin.x && p.x < b.pmax.x
            && p.y >= b.pmin.y && p.y < b.pmax.y
            && p.z >= b.pmin.z && p.z < b.pmax.z;
    }

    // Pad the bounding box by a constant factor in all dimensions
    template<typename T, typename U>
    constexpr base_bounds3<T> expand(const base_bounds3<T>& b, U delta)
    {
        return base_bounds3<T>(b.pmin - base_vec3<T>((T)delta), b.pmax + base_vec3<T>((T)delta));
    }

    // Type definitions
    using bounds3 = base_bounds3<f32>;
    using ibounds3 = base_bounds3<int>;

}
