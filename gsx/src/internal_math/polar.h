#pragma once

#include "vec2.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    // polar coordinates
    class polar_t
    {
    public:
        f32 r;
        f32 theta;

        constexpr polar_t()
            : r(0), theta(0)
        {}

        constexpr polar_t(f32 r, f32 theta)
            : r(r), theta(theta)
        {}

        polar_t(const vec2& cartesian);

        std::string to_string() const;

        friend std::ostream& operator<<(std::ostream& os, const polar_t& p);

        constexpr bool operator==(const polar_t& p) const
        {
            return r == p.r && theta == p.theta;
        }

        constexpr bool operator!=(const polar_t& p) const
        {
            return r != p.r || theta != p.theta;
        }

        const vec2 cartesian() const;

    };

}
