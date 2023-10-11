#include "circle.h"

namespace tef::math
{

    circle_t::circle_t(const bounds2& b)
        : center((b.pmin + b.pmax) * .5f),
        radius(inside(center, b) ? distance(center, b.pmax) : 0)
    {}

    std::string circle_t::to_string() const
    {
        return std::format(
            "[center={}, radius={}]",
            center.to_string(),
            str::from_number(radius)
        );
    }

    std::ostream& operator<<(std::ostream& os, const circle_t& r)
    {
        os << r.to_string();
        return os;
    }

    vec2 circle_t::at(f32 angle) const
    {
        return center + radius * vec2(math::cos(angle), math::sin(angle));
    }

}
