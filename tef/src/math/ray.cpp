#include "ray.h"

namespace tef::math
{

    ray_t::ray_t(const vec3& o, const vec3& d)
        : o(o), d(d)
    {}

    vec3 ray_t::operator()(float t) const
    {
        return o + d * t;
    }

    ray_t::operator std::string() const
    {
        return "[o=" + std::string(o) + ", d=" + std::string(d) + "]";
    }

    std::ostream& operator<<(std::ostream& os, const ray_t& r)
    {
        os << std::string(r);
        return os;
    }

}
