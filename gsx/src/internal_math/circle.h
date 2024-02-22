#pragma once

#include "vec2.h"
#include "bounds2.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<std::floating_point T>
    class base_circle_t
    {
    public:
        base_vec2<T> center;
        T radius;

        constexpr base_circle_t()
            : center(base_vec2<T>(0)), radius(1)
        {}

        constexpr base_circle_t(const base_vec2<T>& center, T radius)
            : center(center), radius(radius)
        {}

        template<std::floating_point U>
        constexpr operator base_circle_t<U>() const
        {
            return base_circle_t<U>((base_vec2<U>)center, (U)radius);
        }

        // construct a circle that bounds a given bounding box
        base_circle_t(const base_bounds2<T>& b)
            : center((b.pmin + b.pmax) * .5f),
            radius(inside(center, b) ? distance(center, b.pmax) : 0)
        {}

        std::string to_string() const
        {
            return std::format(
                "[center={}, radius={}]",
                center.to_string(),
                str::from_number(radius)
            );
        }

        friend std::ostream& operator<<(
            std::ostream& os,
            const base_circle_t& c
            )
        {
            os << c.to_string();
            return os;
        }

        constexpr bool operator==(const base_circle_t& c) const
        {
            return center == c.center && radius == c.radius;
        }

        constexpr bool operator!=(const base_circle_t& c) const
        {
            return center != c.center || radius != c.radius;
        }

        constexpr base_bounds2<T> bounds() const
        {
            return base_bounds2<T>(center - radius, center + radius);
        }

        // a point on the circle at a given angle
        base_vec2<T> at(T angle) const
        {
            return center + radius * unit_at(angle);
        }

        // a point on the unit circle at a given angle
        static base_vec2<T> unit_at(T angle)
        {
            return base_vec2<T>(math::cos(angle), math::sin(angle));
        }

    };

    template<std::floating_point T>
    inline bool inside(const base_vec2<T>& p, const base_circle_t<T>& c)
    {
        return distance_squared(p, c.center) <= squared(c.radius);
    }

    template<std::floating_point T>
    inline bool overlaps(const base_circle_t<T>& c1, const base_circle_t<T>& c2)
    {
        return
            distance_squared(c1.center, c2.center)
            <= squared(c1.radius + c2.radius);
    }

    template<std::floating_point T>
    inline bool overlaps(const base_circle_t<T>& c, const base_bounds2<T>& b)
    {
        return inside(b.pmin, c)
            || inside(vec2(b.pmax.x, b.pmin.y), c)
            || inside(vec2(b.pmin.x, b.pmax.y), c)
            || inside(b.pmax, c);
    }

    template<std::floating_point T>
    inline bool overlaps(const base_bounds2<T>& b, const base_circle_t<T>& c)
    {
        return overlaps(c, b);
    }

    using circle_t = base_circle_t<f32>;
    using dcircle_t = base_circle_t<f64>;

}
