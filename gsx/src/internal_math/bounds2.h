#pragma once

#include <iterator>

#include "vec2.h"
#include "utils.h"
#include "../internal_common/all.h"

namespace gsx::math
{

    template<typename T>
    class base_bounds2
    {
    public:
        base_vec2<T> pmin, pmax;

        constexpr base_bounds2()
            : pmin(base_vec2<T>(std::numeric_limits<T>::max())),
            pmax(base_vec2<T>(std::numeric_limits<T>::lowest()))
        {}

        constexpr base_bounds2(const base_vec2<T>& p)
            : pmin(p), pmax(p)
        {}

        constexpr base_bounds2(const base_vec2<T>& p1, const base_vec2<T>& p2)
            : pmin(min(p1, p2)), pmax(max(p1, p2))
        {}

        template<typename U>
        constexpr operator base_bounds2<U>() const
        {
            return base_bounds2<U>((base_vec2<U>)pmin, (base_vec2<U>)pmax);
        }

        std::string to_string() const
        {
            return std::format(
                "[pmin={}, pmax={}]",
                pmin.to_string(),
                pmax.to_string()
            );
        }

        friend std::ostream& operator<<(
            std::ostream& os,
            const base_bounds2<T>& b
            )
        {
            os << b.to_string();
            return os;
        }

        constexpr const base_vec2<T>& operator[](i32 i) const
        {
            if (i == 0) return pmin;
            return pmax;
        }

        constexpr base_vec2<T>& operator[](i32 i)
        {
            if (i == 0) return pmin;
            return pmax;
        }

        constexpr bool operator==(const base_bounds2<T>& b) const
        {
            return pmin == b.pmin && pmax == b.pmax;
        }

        constexpr bool operator!=(const base_bounds2<T>& b) const
        {
            return pmin != b.pmin || pmax != b.pmax;
        }

        // vector along the box diagonal from the minimum point to the maximum
        // point
        constexpr base_vec2<T> diagonal() const
        {
            return pmax - pmin;
        }

        constexpr T area() const
        {
            base_vec2<T> d = diagonal();
            return d.x * d.y;
        }

        // index of which of the axes is longest
        constexpr i32 max_extent() const
        {
            base_vec2<T> d = diagonal();
            if (d.x > d.y)
                return 0;
            else
                return 1;
        }

        // linear interpolation between the corners of the box by the given
        // amount in each dimension
        constexpr base_vec2<T> lerp(const base_vec2<T>& t) const
        {
            return base_vec2<T>(
                mix(pmin.x, pmax.x, t.x),
                mix(pmin.y, pmax.y, t.y)
            );
        }

        // the continuous position of a point relative to the corners of the
        // box, where a point at the minimum corner has offset (0, 0), a point
        // at the maximum corner has offset (1, 1), and so forth.
        constexpr base_vec2<T> offset_of(const base_vec2<T>& p) const
        {
            base_vec2<T> o = p - pmin;
            if (pmax.x > pmin.x) o.x /= pmax.x - pmin.x;
            if (pmax.y > pmin.y) o.y /= pmax.y - pmin.y;
            return o;
        }

    };

    template<typename T>
    constexpr base_bounds2<T> union_(
        const base_bounds2<T>& b,
        const base_vec2<T>& p
    )
    {
        return base_bounds2<T>(min(b.pmin, p), max(b.pmax, p));
    }

    template<typename T>
    constexpr base_bounds2<T> union_(
        const base_bounds2<T>& b1,
        const base_bounds2<T>& b2
    )
    {
        return base_bounds2<T>(min(b1.pmin, b2.pmin), max(b1.pmax, b2.pmax));
    }

    template<typename T>
    constexpr base_bounds2<T> intersect(
        const base_bounds2<T>& b1,
        const base_bounds2<T>& b2
    )
    {
        return base_bounds2<T>(max(b1.pmin, b2.pmin), min(b1.pmax, b2.pmax));
    }

    template<typename T>
    constexpr bool overlaps(
        const base_bounds2<T>& b1,
        const base_bounds2<T>& b2
    )
    {
        return b1.pmax.x >= b2.pmin.x && b1.pmin.x <= b2.pmax.x
            && b1.pmax.y >= b2.pmin.y && b1.pmin.y <= b2.pmax.y;
    }

    template<typename T>
    constexpr bool inside(
        const base_vec2<T>& p,
        const base_bounds2<T>& b
    )
    {
        return p.x >= b.pmin.x && p.x <= b.pmax.x
            && p.y >= b.pmin.y && p.y <= b.pmax.y;
    }

    // the inside_exclusive() variant of inside() doesn't consider points on the
    // upper boundary to be inside the bounds. it is mostly useful with
    // integer-typed bounds.
    template<typename T>
    constexpr bool inside_exclusive(
        const base_vec2<T>& p,
        const base_bounds2<T>& b
    )
    {
        return p.x >= b.pmin.x && p.x < b.pmax.x
            && p.y >= b.pmin.y && p.y < b.pmax.y;
    }

    // pad the bounding box by a constant factor in all dimensions
    template<typename T, typename U>
    constexpr base_bounds2<T> expand(const base_bounds2<T>& b, U delta)
    {
        return base_bounds2<T>(
            b.pmin - base_vec2<T>((T)delta), b.pmax + base_vec2<T>((T)delta)
        );
    }

    using bounds2 = base_bounds2<f32>;
    using dbounds2 = base_bounds2<f64>;
    using ibounds2 = base_bounds2<int>;

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
