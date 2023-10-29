#include "spherical.h"

namespace gsx::math
{

    spherical_t::spherical_t(const vec3& cartesian)
    {
        r = length(cartesian);
        theta = atan(r, cartesian.z);
        phi = atan(cartesian.y, cartesian.x);
    }

    std::string spherical_t::to_string() const
    {
        return std::format(
            "[r={}, theta={}, phi={}]",
            str::from_number(r),
            str::from_number(theta),
            str::from_number(phi)
        );
    }

    std::ostream& operator<<(std::ostream& os, const spherical_t& s)
    {
        os << s.to_string();
        return os;
    }

    const vec3 spherical_t::cartesian() const
    {
        const f32 sin_theta = math::sin(theta);
        return r * vec3(
            sin_theta * math::cos(phi),
            sin_theta * math::sin(phi),
            math::cos(theta)
        );
    }

}
