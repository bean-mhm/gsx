#pragma once

#include "vec3.h"
#include "utils.h"

#include "../str/utils.h"

namespace tef::math
{

    class ray_t
    {
    public:
        vec3 o;
        vec3 d;

        // Constructors
        constexpr ray_t(const vec3& o, const vec3& d)
            : o(o), d(d)
        {}

        // Evaluate point along the ray
        constexpr vec3 operator()(float t) const
        {
            return o + d * t;
        }

        // String
        constexpr std::string to_string() const
        {
            return std::format(
                "[o={}, d={}]",
                o.to_string(),
                d.to_string()
            );
        }

        // Print
        friend std::ostream& operator<<(std::ostream& os, const ray_t& v);

    };

}
