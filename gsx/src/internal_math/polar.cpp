#include "polar.h"

namespace gsx::math
{

    polar_t::polar_t(const vec2& cartesian)
    {
        r = length(cartesian);

        theta = atan(cartesian.y, cartesian.x);
        if (theta < 0)
            theta += tau;
    }

    std::string polar_t::to_string() const
    {
        return std::format(
            "[r={}, theta={}]",
            str::from_number(r),
            str::from_number(theta)
        );
    }

    std::ostream& operator<<(std::ostream& os, const polar_t& p)
    {
        os << p.to_string();
        return os;
    }

    const vec2 polar_t::cartesian() const
    {
        return r * vec2(math::cos(theta), math::sin(theta));
    }

}
