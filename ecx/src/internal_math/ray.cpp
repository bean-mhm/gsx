#include "ray.h"

namespace ecx::math
{

    std::string ray_t::to_string() const
    {
        return std::format(
            "[o={}, d={}]",
            o.to_string(),
            d.to_string()
        );
    }

    std::ostream& operator<<(std::ostream& os, const ray_t& r)
    {
        os << r.to_string();
        return os;
    }

}
