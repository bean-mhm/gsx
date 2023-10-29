#pragma once

#include "vec3.h"
#include "utils.h"
#include "../internal_common/all.h"
#include "../internal_str/all.h"

namespace gsx::math
{

    // Spherical coordinates
    class spherical_t
    {
    public:
        f32 r;
        f32 theta;
        f32 phi;

        // Constructors

        constexpr spherical_t()
            : r(0), theta(0), phi(0)
        {}

        constexpr spherical_t(f32 r, f32 theta, f32 phi)
            : r(r), theta(theta), phi(phi)
        {}

        spherical_t(const vec3& cartesian);

        // String
        std::string to_string() const;

        // Print
        friend std::ostream& operator<<(std::ostream& os, const spherical_t& s);

        // this == spherical
        constexpr bool operator==(const spherical_t& s) const
        {
            return r == s.r && theta == s.theta && phi == s.phi;
        }

        // this != spherical
        constexpr bool operator!=(const spherical_t& s) const
        {
            return r != s.r || theta != s.theta || phi != s.phi;
        }

        // Cartesian coordinates
        const vec3 cartesian() const;

    };

}
