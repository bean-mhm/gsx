#include "ray.h"

namespace tef::math
{

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
