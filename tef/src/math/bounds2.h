#pragma once

#include <iterator>

#include "vec2.h"
#include "utils.h"

namespace tef::math
{

    template <typename T>
    class bounds2_base
    {
    public:
        vec2_base<T> pmin, pmax;

        // Constructors

        constexpr bounds2_base()
            : pmin(vec2_base<T>(std::numeric_limits<T>::max())),
            pmax(vec2_base<T>(std::numeric_limits<T>::lowest()))
        {}

        constexpr bounds2_base(const vec2_base<T>& p)
            : pmin(p), pmax(p)
        {}

        constexpr bounds2_base(const vec2_base<T>& p1, const vec2_base<T>& p2)
            : pmin(min(p1, p2)), pmax(max(p1, p2))
        {}

        // Type casting
        template <typename U>
        constexpr operator bounds2_base<U>() const
        {
            return bounds2_base<U>((vec2_base<U>)pmin, (vec2_base<U>)pmax);
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
        friend std::ostream& operator<<(std::ostream& os, const bounds2_base<T>& b)
        {
            os << b.to_string();
            return os;
        }

        // Access by index (const reference)
        constexpr const vec2_base<T>& operator[](int i) const
        {
            if (i == 0) return pmin;
            return pmax;
        }

        // Access by index (reference)
        constexpr vec2_base<T>& operator[](int i)
        {
            if (i == 0) return pmin;
            return pmax;
        }

        // The vector along the box diagonal from the minimum point to the maximum point
        constexpr vec2_base<T> diagonal() const
        {
            return pmax - pmin;
        }

        // The area of the box
        constexpr T area() const
        {
            vec2_base<T> d = diagonal();
            return d.x * d.y;
        }

        // The index of which of the axes is longest
        constexpr int max_extent() const
        {
            vec2_base<T> d = diagonal();
            if (d.x > d.y)
                return 0;
            else
                return 1;
        }

        // Linear interpolation between the corners of the box by the given amount in each dimension
        constexpr vec2_base<T> lerp(const vec2_base<T>& t) const
        {
            return vec2_base<T>(
                mix(pmin.x, pmax.x, t.x),
                mix(pmin.y, pmax.y, t.y)
            );
        }

        // The continuous position of a point relative to the corners of the box, where a point at
        // the minimum corner has offset (0, 0), a point at the maximum corner has offset (1, 1),
        // and so forth.
        constexpr vec2_base<T> offset_of(const vec2_base<T>& p) const
        {
            vec2_base<T> o = p - pmin;
            if (pmax.x > pmin.x) o.x /= pmax.x - pmin.x;
            if (pmax.y > pmin.y) o.y /= pmax.y - pmin.y;
            return o;
        }

        // The center and radius of a sphere that bounds the bounding box
        constexpr void bounding_sphere(vec2_base<T>& center, float& radius) const
        {
            center = (pmin + pmax) / 2;
            radius = inside(center, *this) ? distance(center, pmax) : 0;
        }

    };

    // Bounds + point
    template <typename T>
    constexpr bounds2_base<T> union_(const bounds2_base<T>& b, const vec2_base<T>& p)
    {
        return bounds2_base<T>(min(b.pmin, p), max(b.pmax, p));
    }

    // Bounds + bounds
    template <typename T>
    constexpr bounds2_base<T> union_(const bounds2_base<T>& b1, const bounds2_base<T>& b2)
    {
        return bounds2_base<T>(min(b1.pmin, b2.pmin), max(b1.pmax, b2.pmax));
    }

    // Bounds * bounds
    template <typename T>
    constexpr bounds2_base<T> intersect(const bounds2_base<T>& b1, const bounds2_base<T>& b2)
    {
        return bounds2_base<T>(max(b1.pmin, b2.pmin), min(b1.pmax, b2.pmax));
    }

    // Check if two bounding boxes overlap
    template <typename T>
    constexpr bool overlaps(const bounds2_base<T>& b1, const bounds2_base<T>& b2)
    {
        return b1.pmax.x >= b2.pmin.x && b1.pmin.x <= b2.pmax.x
            && b1.pmax.y >= b2.pmin.y && b1.pmin.y <= b2.pmax.y;
    }

    // Check if a point is inside a bounding box
    template <typename T>
    constexpr bool inside(const vec2_base<T>& p, const bounds2_base<T>& b)
    {
        return p.x >= b.pmin.x && p.x <= b.pmax.x
            && p.y >= b.pmin.y && p.y <= b.pmax.y;
    }

    // The inside_exclusive() variant of inside() doesn't consider points on the upper boundary to
    // be inside the bounds. It is mostly useful with integer-typed bounds.
    template <typename T>
    constexpr bool inside_exclusive(const vec2_base<T>& p, const bounds2_base<T>& b)
    {
        return p.x >= b.pmin.x && p.x < b.pmax.x
            && p.y >= b.pmin.y && p.y < b.pmax.y;
    }

    // Pad the bounding box by a constant factor in all dimensions
    template <typename T, typename U>
    constexpr bounds2_base<T> expand(const bounds2_base<T>& b, U delta)
    {
        return bounds2_base<T>(b.pmin - vec2_base<T>(delta), b.pmax + vec2_base<T>(delta));
    }

    // Type definitions
    typedef bounds2_base<float> bounds2;
    typedef bounds2_base<int> ibounds2;

    // ibounds2 iterator
    class ibounds2_iterator : public std::forward_iterator_tag
    {
    public:
        ibounds2_iterator(const ibounds2& b, const ivec2& pt)
            : p(pt), bounds(&b)
        {}

        ibounds2_iterator operator++()
        {
            advance();
            return *this;
        }

        ibounds2_iterator operator++(int)
        {
            ibounds2_iterator old = *this;
            advance();
            return old;
        }

        bool operator==(const ibounds2_iterator& bi) const
        {
            return p == bi.p && bounds == bi.bounds;
        }

        bool operator!=(const ibounds2_iterator& bi) const
        {
            return p != bi.p || bounds != bi.bounds;
        }

        ivec2 operator*() const
        {
            return p;
        }

    private:
        ivec2 p;
        const ibounds2* bounds;

        void advance()
        {
            ++p.x;
            if (p.x == bounds->pmax.x)
            {
                p.x = bounds->pmin.x;
                ++p.y;
            }
        }

    };

}
