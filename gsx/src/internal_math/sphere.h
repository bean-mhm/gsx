#pragma once

#include "vec3.h"
#include "bounds3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    template<std::floating_point T>
    class base_sphere_t
    {
    public:
        base_vec3<T> center;
        T radius;

        constexpr base_sphere_t()
            : center(base_vec3<T>(0)), radius(1)
        {}

        constexpr base_sphere_t(const base_vec3<T>& center, T radius)
            : center(center), radius(radius)
        {}

        template<std::floating_point U>
        constexpr operator base_sphere_t<U>() const
        {
            return base_sphere_t<U>((base_vec3<U>)center, (U)radius);
        }

        // construct a sphere that bounds a given bounding box
        base_sphere_t(const base_bounds3<T>& b)
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
            const base_sphere_t& s
            )
        {
            os << s.to_string();
            return os;
        }

        constexpr bool operator==(const base_sphere_t& s) const
        {
            return center == s.center && radius == s.radius;
        }

        constexpr bool operator!=(const base_sphere_t& s) const
        {
            return center != s.center || radius != s.radius;
        }

        constexpr base_bounds3<T> bounds() const
        {
            return base_bounds3<T>(center - radius, center + radius);
        }

        // a point on the sphere at given theta and phi angles
        base_vec3<T> at(T theta, T phi) const
        {
            return center + radius * unit_at(theta, phi);
        }

        // a point on the unit sphere at given theta and phi angles
        static base_vec3<T> unit_at(T theta, T phi)
        {
            const T sin_theta = math::sin(theta);
            return base_vec3<T>(
                sin_theta * math::cos(phi),
                sin_theta * math::sin(phi),
                math::cos(theta)
            );
        }

    };

    template<std::floating_point T>
    inline bool inside(const base_vec3<T>& p, const base_sphere_t<T>& s)
    {
        return distance_squared(p, s.center) <= squared(s.radius);
    }

    template<std::floating_point T>
    inline bool overlaps(const base_sphere_t<T>& s1, const base_sphere_t<T>& s2)
    {
        return
            distance_squared(s1.center, s2.center)
            <= squared(s1.radius + s2.radius);
    }

    template<std::floating_point T>
    inline bool overlaps(const base_sphere_t<T>& s, const base_bounds3<T>& b)
    {
        return inside(b.pmin, s)
            || inside(base_vec3<T>(b.pmax.x, b.pmin.y, b.pmin.z), s)
            || inside(base_vec3<T>(b.pmin.x, b.pmax.y, b.pmin.z), s)
            || inside(base_vec3<T>(b.pmax.x, b.pmax.y, b.pmin.z), s)
            || inside(base_vec3<T>(b.pmin.x, b.pmin.y, b.pmax.z), s)
            || inside(base_vec3<T>(b.pmax.x, b.pmin.y, b.pmax.z), s)
            || inside(base_vec3<T>(b.pmin.x, b.pmax.y, b.pmax.z), s)
            || inside(b.pmax, s);
    }

    template<std::floating_point T>
    inline bool overlaps(const base_bounds3<T>& b, const base_sphere_t<T>& s)
    {
        return overlaps(s, b);
    }

    using sphere_t = base_sphere_t<f32>;
    using dsphere_t = base_sphere_t<f64>;

}
