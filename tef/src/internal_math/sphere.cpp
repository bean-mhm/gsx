#include "sphere.h"

namespace tef::math
{

    sphere_t::sphere_t(const bounds3& b)
        : center((b.pmin + b.pmax) * .5f),
        radius(inside(center, b) ? distance(center, b.pmax) : 0)
    {}

    std::string sphere_t::to_string() const
    {
        return std::format(
            "[center={}, radius={}]",
            center.to_string(),
            str::from_number(radius)
        );
    }

    std::ostream& operator<<(std::ostream& os, const sphere_t& r)
    {
        os << r.to_string();
        return os;
    }

    vec3 sphere_t::at(f32 theta, f32 phi) const
    {
        const f32 sin_theta = math::sin(theta);
        return center + radius * vec3(
            sin_theta * math::cos(phi),
            sin_theta * math::sin(phi),
            math::cos(theta)
        );
    }

}
