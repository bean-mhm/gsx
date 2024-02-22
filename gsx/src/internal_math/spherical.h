#pragma once

#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    // spherical coordinates
    template<std::floating_point T>
    class base_spherical_t
    {
    public:
        T r;
        T theta;
        T phi;

        constexpr base_spherical_t()
            : r(0), theta(0), phi(0)
        {}

        constexpr base_spherical_t(T r, T theta, T phi)
            : r(r), theta(theta), phi(phi)
        {}

        template<std::floating_point U>
        constexpr operator base_spherical_t<U>() const
        {
            return base_spherical_t<U>((U)r, (U)theta, (U)phi);
        }

        base_spherical_t(const base_vec3<T>& cartesian)
        {
            r = length(cartesian);
            theta = atan(r, cartesian.z);
            phi = atan(cartesian.y, cartesian.x);
        }

        std::string to_string() const
        {
            return std::format(
                "[r={}, theta={}, phi={}]",
                str::from_number(r),
                str::from_number(theta),
                str::from_number(phi)
            );
        }

        friend std::ostream& operator<<(
            std::ostream& os,
            const base_spherical_t& s
            )
        {
            os << s.to_string();
            return os;
        }

        constexpr bool operator==(const base_spherical_t& s) const
        {
            return r == s.r && theta == s.theta && phi == s.phi;
        }

        constexpr bool operator!=(const base_spherical_t& s) const
        {
            return r != s.r || theta != s.theta || phi != s.phi;
        }

        const base_vec3<T> cartesian() const
        {
            const T sin_theta = math::sin(theta);
            return r * base_vec3<T>(
                sin_theta * math::cos(phi),
                sin_theta * math::sin(phi),
                math::cos(theta)
            );
        }

    };

    using spherical_t = base_spherical_t<f32>;
    using dspherical_t = base_spherical_t<f64>;

}
