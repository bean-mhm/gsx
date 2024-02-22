#pragma once

#include "vec2.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    // polar coordinates
    template<std::floating_point T>
    class base_polar_t
    {
    public:
        T r;
        T theta;

        constexpr base_polar_t()
            : r(0), theta(0)
        {}

        constexpr base_polar_t(T r, T theta)
            : r(r), theta(theta)
        {}

        template<std::floating_point U>
        constexpr operator base_polar_t<U>() const
        {
            return base_polar_t<U>((U)r, (U)theta);
        }

        base_polar_t(const base_vec2<T>& cartesian)
        {
            r = length(cartesian);

            theta = atan(cartesian.y, cartesian.x);
            if (theta < 0)
                theta += tau<T>;
        }

        std::string to_string() const
        {
            return std::format(
                "[r={}, theta={}]",
                str::from_number(r),
                str::from_number(theta)
            );
        }

        friend std::ostream& operator<<(std::ostream& os, const base_polar_t& p)
        {
            os << p.to_string();
            return os;
        }

        constexpr bool operator==(const base_polar_t& p) const
        {
            return r == p.r && theta == p.theta;
        }

        constexpr bool operator!=(const base_polar_t& p) const
        {
            return r != p.r || theta != p.theta;
        }

        const base_vec2<T> cartesian() const
        {
            return r * base_vec2<T>(math::cos(theta), math::sin(theta));
        }

    };

    using polar_t = base_polar_t<f32>;
    using dpolar_t = base_polar_t<f64>;

}
