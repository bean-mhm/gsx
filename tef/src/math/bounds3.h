#pragma once

#include "vec3.h"
#include "utils.h"

namespace tef::math
{

    template <typename T>
    class bounds3_base
    {
    public:
        vec3_base<T> pmin, pmax;

        // Constructors

        constexpr bounds3_base()
            : pmin(vec3_base<T>(std::numeric_limits<T>::max())),
            pmax(vec3_base<T>(std::numeric_limits<T>::lowest()))
        {}

        constexpr bounds3_base(const vec3_base<T>& p)
            : pmin(p), pmax(p)
        {}

        constexpr bounds3_base(const vec3_base<T>& p1, const vec3_base<T>& p2)
            : pmin(min(p1, p2)), pmax(max(p1, p2))
        {}

        // Type casting
        template <typename U>
        constexpr operator bounds3_base<U>() const
        {
            return bounds3_base<U>((vec3_base<U>)pmin, (vec3_base<U>)pmax);
        }

        // String
        constexpr std::string to_string() const
        {
            return std::format(
                "[pmin={}, pmax={}]",
                pmin.to_string(),
                pmax.to_string()
            );
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const bounds3_base<T>& b)
        {
            os << b.to_string();
            return os;
        }

        // Access by index (const reference)
        constexpr const vec3_base<T>& operator[](int i) const
        {
            if (i == 0) return pmin;
            return pmax;
        }

        // Access by index (reference)
        constexpr vec3_base<T>& operator[](int i)
        {
            if (i == 0) return pmin;
            return pmax;
        }

        // The coordinates of one of the eight corners of the bounding box
        constexpr vec3_base<T> corner(int i) const
        {
            return vec3_base<T>(
                (*this)[(i & 1)].x,
                (*this)[(i & 2) ? 1 : 0].y,
                (*this)[(i & 4) ? 1 : 0].z
            );
        }

        // The vector along the box diagonal from the minimum point to the maximum point
        constexpr vec3_base<T> diagonal() const
        {
            return pmax - pmin;
        }

        // The surface area of the six faces of the box
        constexpr T surface_area() const
        {
            vec3_base<T> d = diagonal();
            return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
        }

        // The volume of the bounding box
        constexpr T volume() const
        {
            vec3_base<T> d = diagonal();
            return d.x * d.y * d.z;
        }

        // The index of which of the axes is longest
        constexpr int max_extent() const
        {
            vec3_base<T> d = diagonal();
            if (d.x > d.y && d.x > d.z)
                return 0;
            else if (d.y > d.z)
                return 1;
            else
                return 2;
        }

        // Linear interpolation between the corners of the box by the given amount in each
        // dimension
        constexpr vec3_base<T> lerp(const vec3_base<T>& t) const
        {
            return vec3_base<T>(
                mix(pmin.x, pmax.x, t.x),
                mix(pmin.y, pmax.y, t.y),
                mix(pmin.z, pmax.z, t.z)
            );
        }

        // The continuous position of a point relative to the corners of the box, where a point at
        // the minimum corner has offset (0, 0, 0), a point at the maximum corner has offset
        // (1, 1, 1), and so forth.
        constexpr vec3_base<T> offset_of(const vec3_base<T>& p) const
        {
            vec3_base<T> o = p - pmin;
            if (pmax.x > pmin.x) o.x /= pmax.x - pmin.x;
            if (pmax.y > pmin.y) o.y /= pmax.y - pmin.y;
            if (pmax.z > pmin.z) o.z /= pmax.z - pmin.z;
            return o;
        }

        // The center and radius of a sphere that bounds the bounding box
        constexpr void bounding_sphere(vec3_base<T>& center, float& radius) const
        {
            center = (pmin + pmax) / 2;
            radius = inside(center, *this) ? distance(center, pmax) : 0;
        }

    };

    // Bounds + point
    template <typename T>
    constexpr bounds3_base<T> union_(const bounds3_base<T>& b, const vec3_base<T>& p)
    {
        return bounds3_base<T>(min(b.pmin, p), max(b.pmax, p));
    }

    // Bounds + bounds
    template <typename T>
    constexpr bounds3_base<T> union_(const bounds3_base<T>& b1, const bounds3_base<T>& b2)
    {
        return bounds3_base<T>(min(b1.pmin, b2.pmin), max(b1.pmax, b2.pmax));
    }

    // Bounds * bounds
    template <typename T>
    constexpr bounds3_base<T> intersect(const bounds3_base<T>& b1, const bounds3_base<T>& b2)
    {
        return bounds3_base<T>(max(b1.pmin, b2.pmin), min(b1.pmax, b2.pmax));
    }

    // Check if two bounding boxes overlap
    template <typename T>
    constexpr bool overlaps(const bounds3_base<T>& b1, const bounds3_base<T>& b2)
    {
        return b1.pmax.x >= b2.pmin.x && b1.pmin.x <= b2.pmax.x
            && b1.pmax.y >= b2.pmin.y && b1.pmin.y <= b2.pmax.y
            && b1.pmax.z >= b2.pmin.z && b1.pmin.z <= b2.pmax.z;
    }

    // Check if a point is inside a bounding box
    template <typename T>
    constexpr bool inside(const vec3_base<T>& p, const bounds3_base<T>& b)
    {
        return p.x >= b.pmin.x && p.x <= b.pmax.x
            && p.y >= b.pmin.y && p.y <= b.pmax.y
            && p.z >= b.pmin.z && p.z <= b.pmax.z;
    }

    // The inside_exclusive() variant of inside() doesn't consider points on the upper boundary to
    // be inside the bounds. It is mostly useful with integer-typed bounds.
    template <typename T>
    constexpr bool inside_exclusive(const vec3_base<T>& p, const bounds3_base<T>& b)
    {
        return p.x >= b.pmin.x && p.x < b.pmax.x
            && p.y >= b.pmin.y && p.y < b.pmax.y
            && p.z >= b.pmin.z && p.z < b.pmax.z;
    }

    // Pad the bounding box by a constant factor in all dimensions
    template <typename T, typename U>
    constexpr bounds3_base<T> expand(const bounds3_base<T>& b, U delta)
    {
        return bounds3_base<T>(b.pmin - vec3_base<T>(delta), b.pmax + vec3_base<T>(delta));
    }

    // Type definitions
    using bounds3 = bounds3_base<float>;
    using ibounds3 = bounds3_base<int>;

}
