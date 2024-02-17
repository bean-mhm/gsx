#pragma once

#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    // spherical coordinates
    class spherical_t
    {
    public:
        f32 r;
        f32 theta;
        f32 phi;

        constexpr spherical_t()
            : r(0), theta(0), phi(0)
        {}

        constexpr spherical_t(f32 r, f32 theta, f32 phi)
            : r(r), theta(theta), phi(phi)
        {}

        spherical_t(const vec3& cartesian);

        std::string to_string() const;

        friend std::ostream& operator<<(std::ostream& os, const spherical_t& s);

        constexpr bool operator==(const spherical_t& s) const
        {
            return r == s.r && theta == s.theta && phi == s.phi;
        }

        constexpr bool operator!=(const spherical_t& s) const
        {
            return r != s.r || theta != s.theta || phi != s.phi;
        }

        const vec3 cartesian() const;

    };

}
