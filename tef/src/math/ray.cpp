#include "ray.h"

namespace tef::math
{

    std::ostream& operator<<(std::ostream& os, const ray_t& r)
    {
        os << r.to_string();
        return os;
    }

}
